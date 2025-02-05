add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE nddb::libnddb Catch2::Catch2WithMain)