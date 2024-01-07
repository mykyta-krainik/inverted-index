project(google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(google_tests_run inverted_index_test.cpp)

target_link_libraries(google_tests_run gtest)
target_link_libraries(google_tests_run inverted_index_lib)