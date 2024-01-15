project(thread_pool_lib)

set(HEADER_FILES thread_pool.h)
set(SOURCE_FILES thread_pool.cpp)

add_library(thread_pool_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
