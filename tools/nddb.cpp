#include <libnddb/libnddb.hpp>

#include <sys/ptrace.h>

#include <iostream>
#include <string_view>

namespace {
    pid_t attach(int argc, const char** argv) {
        pid_t pid = 0;
        // if the user passes pid or a running 
        // process
        // nddb -p <pid>
        if(argc == 3 && argv[1] == std::string_view("-p")) {

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

    pid_t pid = attach(argc, argv);
  
}