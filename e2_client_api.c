//
// Created by Eugenio Moro on 28/06/22.
//

#include "e2_client_api.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <pb_encode.h>
#include <pb_decode.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "e2.pb.h"
#include "e2prtbf_common.h"
#include "E2_requests.h"

int socket_fd;
struct sockaddr_in servaddr;
E2_request* rqst_p;
pb_ostream_t ostream;
pb_istream_t istream;


bool e2_api_init(char *server_str_addr, int port) {
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr(server_str_addr);
    servaddr.sin_port = htons(port);
    rqst_p = (E2_request*)malloc(E2_request_size);
    if (rqst_p == NULL){
        return false;
    }
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("Socket creation failed...\n");
        return false;
    } else {
        return true;
    }
}

bool e2_api_connect() {
    int ret = connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret == 0){
        ostream = pb_ostream_from_socket(socket_fd);
        istream = pb_istream_from_socket(socket_fd);
        return true;
    } else {
        printf("Socket connection failed with error %d...\n",ret);
        return false;
    }
}

bool e2_api_DUMMY_transaction(E2_dummy_response *rsp) {
    // build and send request
    rqst_p->req_id = E2_REQID_DUMMY1;
    e2_api_connect();
    if(!pb_encode_delimited(&ostream,E2_request_fields,rqst_p)){
        printf("Encoding failed\n");
        return false;
    }

    // wait for answer, write on provided pointer
    istream = pb_istream_from_socket(socket_fd);
    if (pb_decode_delimited(&istream, E2_dummy_response_fields, rsp)){
        printf("dioca: %s\n", rsp->mess_string);
        return true;
    } else {
        printf("Decoding failed");
        return false;
    }
}

void e2_api_register_callback() {
    //TODO
}

void e2_api_teardown() {
    if (rqst_p != NULL){
        free(rqst_p);
    }
}
