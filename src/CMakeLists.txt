add_library(libnddb process.cpp)
add_library(nddb::libnddb ALIAS libnddb)

set_target_properties(libnddb PROPERTIES OUTPUT_NAME nddb)

target_compile_features(libnddb PUBLIC cxx_std_17)

target_include_directories(
    libnddb
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_SOURCE_DIR}/src/include
)

include(GNUInstallDirs)
install(TARGETS libnddb
        EXPORT nddb-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT nddb-targets
    FILE nddb-config.cmake
    NAMESPACE nddb::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nddb
)