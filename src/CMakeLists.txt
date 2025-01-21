add_library(libnddb libnddb.cpp)
add_library(nddb::libnddb ALIAS libnddb)

set_target_properties(libnddb PROPERTIES OUTPUT_NAME nddb)

target_compile_features(libnddb PUBLIC cxx_std_17)

target_include_directories(
    libnddb
    PRIVATE include
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)