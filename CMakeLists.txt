cmake_minimum_required(VERSION 3.27)
project(inverted_index_lib)

set(CMAKE_CXX_STANDARD 23)
set(SOURCE_FILES inverted_index_lib/inverted_index.cpp main.cpp)

add_executable(inverted_index_run ${SOURCE_FILES})

include_directories(inverted_index_lib)
add_subdirectory(inverted_index_lib)

target_link_libraries(inverted_index_run inverted_index_lib)

add_subdirectory(google_tests)
