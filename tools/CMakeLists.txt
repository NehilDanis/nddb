add_executable(nddb nddb.cpp)

target_link_libraries(nddb PRIVATE nddb::libnddb)