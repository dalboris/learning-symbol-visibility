add_library(foo SHARED foo.cpp)
target_compile_definitions(foo PRIVATE -DFOO_EXPORTS)
