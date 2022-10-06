//
// Created by Eugenio Moro on 28/06/22.
//


#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "proto/e2.pb-c.h"

#include <arpa/inet.h>



int main(int argc, char **argv){
    printf("E2 temination emulator started\n");

    // init e2 client api
    int socket_fd;
    int port = 6656;
    struct sockaddr_in servaddr;

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
    //char st[] = "Hi, this is a dummy response!!";
    rsp.mess_string =  "Hi, this is a dummy response!!";
    rsp.result = 1; 

    // encode to buffer
    unsigned buflen = e2_dummy_response__get_packed_size(&rsp);
    void* buf = malloc(buflen);
    e2_dummy_response__pack(&rsp, buf);

    printf("sending buffer\n");
    int sendret = sendto(socket_fd, buf, buflen, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("done, ret is %d\n", sendret);
    /*
    if (e2_api_init(server_str_addr,port)) {
        printf("E2 api initialized\n");
    } else {
        printf("Failed api init\n");
        return -1;
    }

    E2_dummy_response* rsp;// = E2_dummy_response_init_zero;
    rsp = (E2_dummy_response*) malloc(E2_dummy_response_size);
    if (e2_api_DUMMY_transaction(rsp)){
        printf("Dummy transaction successful, message: %s - id: %d\n", rsp->mess_string, rsp->req_id);
    } else {
        printf("Dummy transaction failed\n");
    }

    sleep(3);

    if (e2_api_DUMMY_transaction_type2(rsp)){
        printf("Dummy transaction successful, message: %s - id: %d\n", rsp->mess_string, rsp->req_id);
    } else {
        printf("Dummy transaction failed\n");
    }

    // dummy transaction
    */

}