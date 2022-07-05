cmake_minimum_required(VERSION 2.8)
project(NANOPB_CMAKE_SIMPLE C)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
#set(NANOPB_GENERATE_CPP_APPEND_PATH TRUE)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS} proto)

#nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/e2.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR} common proto)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")
set(PROTO_FILES proto/e2.pb.h proto proto/e2.pb.c)
set(PROTO_SRCS nanopb/pb_common.c nanopb/pb_decode.c nanopb/pb_decode.c)
set(PROTO_HDRS nanopb/pb_common.h nanopb/pb_decode.h nanopb/pb_decode.h nanopb/pb.h)

add_executable(simple simple.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h)
add_executable(gnb_e2server_emu gnb_e2server_emu.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h gnb_e2server_emu.c common/e2prtbf_common.c common/e2prtbf_common.h
        proto/e2.pb.h proto proto/e2.pb.c)
add_executable(e2_term_emu e2term_emu.c ${PROTO_SRCS} ${PROTO_HDRS} E2_requests.h e2_client_api.h e2_client_api.c common/e2prtbf_common.c ${PROTO_FILES}
        nanopb/pb_common.h nanopb/pb_decode.h nanopb/pb_encode.h nanopb/pb.h
        nanopb/pb_common.c nanopb/pb_decode.c nanopb/pb_encode.c)