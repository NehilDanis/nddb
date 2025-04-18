// libedit and printing command line related
#include <editline/readline.h>

// stuff to connect the process
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// std stuff or general cpp related
#include <iostream>
#include <string_view>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

// third party
#include <tl/expected.hpp>

namespace {

    template <typename T>
    using Result = tl::expected<T, std::string>;

    auto make_error(std::string_view error_message) {
        return tl::make_unexpected(error_message.data());
    }

    template<typename T>
    auto expect(Result<T>&& result) -> T {
        if(!result.has_value()) {
            throw std::runtime_error(result.error());
        }
        return std::move(result.value());
    }

    auto attach(int argc, const char** argv) -> Result<pid_t> {
        pid_t pid = 0;
        // if the user passes pid or a running 
        // process
        // nddb -p <pid>
        if(argc == 3 && argv[1] == std::string_view("-p")) {
            pid = std::atoi(argv[2]);
            std::cout << pid << std::endl;
            if(pid <= 0) {
                return make_error("Failed due to invalid pid");
            }
            if(ptrace(PTRACE_ATTACH, pid, /*addr=*/nullptr, /*data=*/nullptr) < 0) {
                std::perror("COuld not attach: ");
                return make_error("Couldn't attach to the process");
            }
        }
        // if the user provided a program name to launch
        else {
            const char* program_path = argv[1];
            if((pid = fork()) < 0) {
                return make_error("Fork failed");
            }
            else if(pid == 0) { // in the child process
                if(ptrace(PTRACE_TRACEME, /*pid=*/0, /*addr=*/nullptr, /*data=*/nullptr) < 0) {
                    return make_error("Tracing the child process failed.");
                }
                if(execlp(program_path, program_path, nullptr) == -1) {
                    return make_error("Exec failed.");
                }
            }
        }

        return pid;
    }

    void resume(pid_t pid) {
        if(ptrace(PTRACE_CONT, pid, /*addr=*/nullptr, /*data=*/nullptr) < 0) {
            std::perror("Could not continue: ");
            std::exit(-1);
        }
    }

    void wait_on_signal(pid_t pid) {
        int wait_status;
        int wait_options = 0;

        if(waitpid(pid, &wait_status, wait_options) < 0) {
            std::perror("waitpid failed.");
            std::exit(-1);
        }
    }

    std::vector<std::string> split(std::string_view str, char delimeter) {

        std::vector<std::string> out{};

        std::stringstream s_stream {std::string{str}};
        std::string item{};
        while(std::getline(s_stream, item, delimeter)) {
            out.emplace_back(item);
        }
        return out;
    }

    bool is_prefix(std::string_view command, std::string_view of) {
        /*
        example: in case of continue command user should be able to enter continue, cont or c and
        in all those cases we should match. This is the behavior of gdb also.
        */

        if(command.length() > of.length()) return false;
        return std::equal(command.begin(), command.end(), of.begin()); 
    }

    void handle_command(pid_t pid, std::string_view line) {
        auto args = split(line, ' '); 
        auto command = args[0];

        if(is_prefix(command, "continue")) {
            resume(pid);
            wait_on_signal(pid);
        }
        else {
            std::cerr << "Unknown command.\n";
        }
    }

}

int main(int argc, const char** argv) {
    if(argc == 1) {
        std::cerr << "No arguments given\n";
        return -1;
    }

    pid_t pid = expect(attach(argc, argv));
    
    int wait_status;
    int wait_options = 0;

    if(waitpid(pid, &wait_status, wait_options) < 0) {
        std::perror("waitpid failed.");
    }

    char* line = nullptr;

    while((line = readline("nddb>")) != nullptr) {
        std::string line_str;
        if(line == std::string_view("")) {
            free(line);
            if(history_length > 0) { // if user enters nothing then we can run the previously ran command
                line_str = history_list()[history_length - 1]->line; 
            }
        }
        else {
            line_str = line;
            add_history(line);
            free(line);
        }

        if(!line_str.empty()) {
            handle_command(pid, line_str);
        }
    }
}