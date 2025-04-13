#ifndef NDDB_PROCESS_HPP
#define NDDB_PROCESS_HPP

#include <filesystem>
#include <memory>
#include <sys/types.h>


namespace nddb {

    class process {
        public:
            process() = delete;
            process(const process& other) = delete;
            process operator=(const process& other) = delete;

            static std::unique_ptr<process> launch(const std::filesystem::path& path_to_executable);
            static std::unique_ptr<process> attach(pid_t pid);

            void resume();
            pid_t get_pid() const { return _pid;}
        private:
            pid_t _pid{0};

    };

}

#endif // NDDB_PROCESS_HPP