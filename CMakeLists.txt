cmake_minimum_required(VERSION 2.8)
project(NANOPB_CMAKE_SIMPLE C)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
#set(NANOPB_GENERATE_CPP_APPEND_PATH TRUE)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/e2.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR} common)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")

add_executable(simple simple.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h)
add_executable(gnb_e2server_emu gnb_e2server_emu.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h gnb_e2server_emu.c common/e2prtbf_common.c common/e2prtbf_common.h)
add_executable(e2_term_emu e2term_emu.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h e2_client_api.h e2_client_api.c common/e2prtbf_common.c common/e2prtbf_common.h)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

print_all_variables()