#include "oai-oran-protolib/builds/ran_messages.pb-c.h"
#include <stdlib.h>
#include "oai-oran-protolib/builds/ran_messages.pb-c.h"
#include <assert.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h> 

typedef struct sockaddr_in sockaddr_in;

void handle_subscription(RANMessage* in_mess);
void handle_indication_request(RANMessage* in_mess,int out_socket, sockaddr_in servaddr);
void handle_indication_response(RANMessage* in_mess, int out_socket, sockaddr_in servaddr);
void handle_control(RANMessage* in_mess);
const char* get_enum_name(RANParameter ran_par_enum);
void ran_write(RANParamMapEntry* target_param_map_entry);
char* ran_read(RANParameter ran_par_enum);
char* itoa(int i);
UeListM* get_ue_list();
void ran_read_new(RANParameter ran_par_enum, RANParamMapEntry* map_entry);