#include <libnddb/libnddb.hpp>

#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <string_view>
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
            if(pid <= 0) {
                return make_error("Failed due to invalid pid");
            }
            if(ptrace(PTRACE_ATTACH, pid, /*addr=*/nullptr, /*data=*/nullptr) < 0) {
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
}