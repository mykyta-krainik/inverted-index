cmake_minimum_required(VERSION 3.27)
project(inverted_index_lib)

set(CMAKE_CXX_STANDARD 23)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC
        inverted_index_lib
        thread_pool_lib
        document_parser_lib
        server_lib
)

add_executable(inverted_index_run inverted_index_lib/inverted_index.cpp)
include_directories(inverted_index_lib)
add_subdirectory(inverted_index_lib)

target_link_libraries(inverted_index_run inverted_index_lib)

add_executable(thread_pool_run thread_pool_lib/thread_pool.cpp)
include_directories(thread_pool_lib)
add_subdirectory(thread_pool_lib)

target_link_libraries(thread_pool_run PUBLIC thread_pool_lib)
target_include_directories(thread_pool_run PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/thread_pool_lib"
        "${PROJECT_SOURCE_DIR}/inverted_index_lib"
)

add_executable(document_parser_run document_parser_lib/document_parser.cpp)
include_directories(document_parser_lib)
add_subdirectory(document_parser_lib)

target_link_libraries(document_parser_run PUBLIC document_parser_lib)
target_include_directories(document_parser_run PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/document_parser_lib"
        "${PROJECT_SOURCE_DIR}/inverted_index_lib"
)

add_executable(server_run server_lib/server.cpp)
include_directories(server_lib)
add_subdirectory(server_lib)

target_link_libraries(server_run PUBLIC server_lib)
target_include_directories(server_run PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/server_lib"
        "${PROJECT_SOURCE_DIR}/inverted_index_lib"
)

add_subdirectory(google_tests)
