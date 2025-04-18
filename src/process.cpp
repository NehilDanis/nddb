#include <libnddb/process.hpp>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>


namespace nddb {
    std::unique_ptr<process> process::launch(const std::filesystem::path& path_to_executable) {

        pid_t pid;

        if((pid = fork()) < 0) {
            // Error: fork failed
        }
        if (pid == 0) { // child process
            if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {
                // Error: trace failed
            }
            if(execlp(path_to_executable.c_str(), path_to_executable.c_str(), nullptr) < 0) {
                // Error: launching failed
            }
        }

        std::unique_ptr<process> proc {new process(pid, /*terminate_on_end*/ true)};

        proc->wait_on_signal();
        return proc;
    }

    std::unique_ptr<process> process::attach(pid_t pid) {


        if(pid <= 0) {
            // Error: invalid pid
        }

        if(ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) < 0) {
            // Error: attach failed
        }

        std::unique_ptr<process> proc {new process(pid, /*terminate_on_end*/ true)};

        proc->wait_on_signal();
        return proc;
    }
}