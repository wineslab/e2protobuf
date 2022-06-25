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
add_executable(gnb_e2_emu gnb_e2_emu.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h gnb_e2_emu.c gnb_connector.c gnb_connector.h common/e2prtbf_common.c common/e2prtbf_common.h)
