//
// Created by Eugenio Moro on 28/06/22.
//

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

int socket_fd;
struct sockaddr_in servaddr;
pb_ostream_t ostream;
pb_istream_t istream;


bool e2_api_init(char *server_str_addr, int port) {
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr(server_str_addr);
    servaddr.sin_port = htons(port);

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed"); 
        exit(EXIT_FAILURE);
    }
    
    E2DummyResponse rsp = E2_DUMMY_RESPONSE__INIT;
    rsp.req_id = 100;
    rsp.req_id = req_id;
    //char st[] = "Hi, this is a dummy response!!";
    rsp.mess_string =  "Hi, this is a dummy response!!";
    rsp.result = 1; 

    // encode to buffer
    unsigned buflen = e2_dummy_response__get_packed_size(&rsp);
    void* buf = malloc(buflen);
    e2_dummy_response__pack(&rsp, buf);

    printf("sending buffer\n");
    int sent_len;
    sendto(socket_fd, buf, buflen, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("done\n");

/*    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("Socket creation failed...\n");
        return false;
    } else {
        //e2_api_connect();
        return true;
    }*/
}

bool e2_api_connect() {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        printf("Socket creation failed...\n");
        return false;
    }
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
        return true;
    } else {
        printf("Decoding failed\n");
        return false;
    }
}

bool e2_api_DUMMY_transaction_type2(E2_dummy_response *rsp) {
    // build and send request
    rqst_p->req_id = E2_REQID_DUMMY2;
    e2_api_connect();
    if(!pb_encode_delimited(&ostream,E2_request_fields,rqst_p)){
        printf("Encoding failed\n");
        return false;
    }

    // wait for answer, write on provided pointer
    istream = pb_istream_from_socket(socket_fd);
    if (pb_decode_delimited(&istream, E2_dummy_response_fields, rsp)){
        return true;
    } else {
        printf("Decoding failed\n");
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
