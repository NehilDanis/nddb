#include <libnddb/libnddb.hpp>

#include <sys/ptrace.h>
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
  
}