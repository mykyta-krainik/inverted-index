project(document_parser_lib)

set(HEADER_FILES document_parser.h)
set(SOURCE_FILES document_parser.cpp)

add_library(document_parser_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})