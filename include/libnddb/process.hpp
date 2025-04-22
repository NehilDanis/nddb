#ifndef NDDB_PROCESS_HPP
#define NDDB_PROCESS_HPP

#include <filesystem>
#include <memory>
#include <sys/types.h>


namespace nddb {

    enum class process_state {
        stopped,
        running,
        exited,
        terminated
    };
    class process {
        public:
            process() = delete;
            process(const process& other) = delete;
            process operator=(const process& other) = delete;

            // destructor
            ~process();

            static std::unique_ptr<process> launch(const std::filesystem::path& path_to_executable);
            static std::unique_ptr<process> attach(pid_t pid);

            process_state state() const { return _state;};

            void resume() {};
            pid_t get_pid() const { return _pid;}
        private:
            process(pid_t pid, bool terminate_on_end) : _pid(pid), _terminate_on_end(terminate_on_end){};
            pid_t _pid{0};
            bool _terminate_on_end{true}; // we should clean the inferior process if we launched it ourselves.
            process_state _state = process_state::stopped;

    };

}

#endif // NDDB_PROCESS_HPP