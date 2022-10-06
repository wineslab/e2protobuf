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
#include "proto/e2.pb-c.h"

#include <arpa/inet.h>

#include "E2_requests.h"

bool e2_api_init(char *server_str_addr, int port);
void e2_api_teardown();
bool e2_api_connect();

bool e2_api_DUMMY_transaction(E2_dummy_response *rsp);
void e2_api_register_callback();
bool e2_api_DUMMY_transaction_type2(E2_dummy_response *rsp);