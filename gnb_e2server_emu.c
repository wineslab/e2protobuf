//
// Created by Eugenio Moro on 24/06/22.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "e2.pb.h"
#include "e2prtbf_common.h"
#include "E2_requests.h"

/* This callback function will be called during the encoding.
 * It will write out any number of FileInfo entries, without consuming unnecessary memory.
 * This is accomplished by fetching the filenames one at a time and encoding them
 * immediately.
 */


void build_send_dummy_response(E2_REQID_t req_id, E2_dummy_response* rsp, int connfd){
    rsp->req_id = 100;
    rsp->req_id = req_id;
    strcpy(rsp->mess_string,"Hi, this is a dummy response!!");
    printf("dioca: %s\n",rsp->mess_string);
    rsp->result = true;
    pb_ostream_t output = pb_ostream_from_socket(connfd);
    if (!pb_encode_delimited(&output, E2_dummy_response_fields, rsp)) {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&output));
    }
    printf("Encoded in %lu bytes\n", output.bytes_written);
}

void build_send_dummy_response2(E2_REQID_t req_id, E2_dummy_response* rsp2, int connfd){
    E2_dummy_response rsp = E2_dummy_response_init_zero;
    rsp.req_id = 100;
    rsp.req_id = req_id;
    strcpy(rsp.mess_string,"Hi, this is a dummy response!!");
    printf("dioca: %s\n",rsp.mess_string);
    rsp.result = true;
    pb_ostream_t output = pb_ostream_from_socket(connfd);
    if (!pb_encode_delimited(&output, E2_dummy_response_fields, &rsp)) {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&output));
    }
    printf("Encoded in %lu bytes\n", output.bytes_written);
}

void ship_response(pb_msgdesc_t msg_descriptor, void* response, int connfd){
    pb_ostream_t output = pb_ostream_from_socket(connfd);
    if (!pb_encode_delimited(&output, &msg_descriptor, response)) {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&output));
    }
    printf("Encoded in %lu bytes\n", output.bytes_written);
}

void handle_connection(int connfd) {

    E2_request request = {};
    pb_istream_t input = pb_istream_from_socket(connfd);
    if (!pb_decode_delimited(&input, E2_request_fields, &request)) {
        printf("Decode failed: %s\n", PB_GET_ERROR(&input));
        return;
    }
    printf("Received E2 request id %d\n", request.req_id);
    switch (request.req_id++) {
        case E2_REQID_DUMMY1:{
            E2_dummy_response* rsp = malloc(E2_dummy_response_size);
            build_send_dummy_response2(E2_REQID_DUMMY1,rsp, connfd);
            //ship_response(E2_dummy_response_msg,rsp,connfd);
            free(rsp);
            break;
        }
        case E2_REQID_DUMMY2:
            break;
        default:
            printf("Unrecognized request id %d\n",request.req_id);
            break;
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    int reuse = 1;

    printf("Starting e2server emulator\n");
    /* Listen on localhost:1234 for TCP connections */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(1234);
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(listenfd, 5) != 0) {
        perror("listen");
        return 1;
    }
    printf("Waiting for TCP connections on localhost:1234...\n");
    for (;;) {
        /* Wait for a client */
        connfd = accept(listenfd, NULL, NULL);

        if (connfd < 0) {
            perror("accept");
            return 1;
        }
        printf("Got connection.\n");
        //sleep(5);
        handle_connection(connfd);

        printf("Closing connection.\n");

        close(connfd);
    }

    return 0;
}