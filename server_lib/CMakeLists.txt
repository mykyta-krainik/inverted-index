project(server_lib)

set(HEADER_FILES server.h)
set(SOURCE_FILES server.cpp)

add_library(server_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})