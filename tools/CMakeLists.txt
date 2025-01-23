add_executable(nddb nddb.cpp)

target_link_libraries(nddb PRIVATE nddb::libnddb)

include(GNUInstallDirs)
install(TARGETS nddb 
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})