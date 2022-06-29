//
// Created by Eugenio Moro on 28/06/22.
//
#include "e2_client_api.h"
#include "e2.pb.h"
#include "e2prtbf_common.h"
#include "E2_requests.h"
#include <stdlib.h>

int main(int argc, char **argv){
    printf("E2 temination emulator started\n");

    // init e2 client api
    char server_str_addr[] = "127.0.0.1";
    int port = 1234;
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

}