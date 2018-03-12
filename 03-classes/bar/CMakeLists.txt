include_directories("${CMAKE_CURRENT_LIST_DIR}/../foo")
add_executable(bar bar.cpp)
target_link_libraries(bar foo)
