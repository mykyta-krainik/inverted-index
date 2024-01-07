project(inverted_index_thread_safe)

set(HEADER_FILES inverted_index.h)
set(SOURCE_FILES inverted_index.cpp)

add_library(inverted_index_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})