#include "gnb_message_handlers.h"

int gnb_id = 0;
int something = 0;



void handle_subscription(RANMessage* in_mess){
    printf("Not implemented\n");
    assert(0!=0);
}
void handle_indication_request(RANMessage* in_mess,int out_socket, sockaddr_in peeraddr){
    printf("Indication request for %lu parameters:\n", in_mess->ran_indication_request->n_target_params);
    for(int par_i=0; par_i<in_mess->ran_indication_request->n_target_params; par_i++){
        printf("\tParameter id %d requested (a.k.a %s)\n",\
        in_mess->ran_indication_request->target_params[par_i],\
        get_enum_name(in_mess->ran_indication_request->target_params[par_i]));
    }
    handle_indication_response(in_mess, out_socket,peeraddr);
}
void handle_indication_response(RANMessage* in_mess, int out_socket, sockaddr_in servaddr){
    // build and fill indication response with target parameters from indication request
    RANIndicationResponse rsp = RAN_INDICATION_RESPONSE__INIT;
    RANParamMapEntry **map;
    void* buf;
    unsigned buflen, i;
    map = malloc(sizeof(RANParamMapEntry*) * in_mess->ran_indication_request->n_target_params);
    for(i=0; i<in_mess->ran_indication_request->n_target_params; i++){
        map[i] = malloc(sizeof(RANParamMapEntry));
        ran_param_map_entry__init(map[i]);
        map[i]->key=in_mess->ran_indication_request->target_params[i];
        map[i]->value = ran_read(map[i]->key);
    }
    rsp.n_param_map=in_mess->ran_indication_request->n_target_params;
    rsp.param_map=map;
    buflen = ran_indication_response__get_packed_size(&rsp);
    buf = malloc(buflen);
    ran_indication_response__pack(&rsp,buf);
    printf("Sending indication response\n");
    unsigned slen = sizeof(servaddr);
    int rev = sendto(out_socket, (const char *)buf, buflen,  
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            slen);
    printf("Sent %d bytes, buflen was %u\n",rev, buflen);
    /*
    printf("Printing buffer for debug pourposes:\n");
    uint8_t* b = (uint8_t*) buf;
    for (int i=0; i<buflen; i++){
        printf(" %hhx ", b[i]);
    }
    printf("\n");
    */
}

void handle_control(RANMessage* in_mess){
    // loop tarhet params and apply
    for(int i=0; i<in_mess->ran_control_request->n_target_param_map; i++){
        printf("Applying target parameter %s with value %s\n",\
        get_enum_name(in_mess->ran_control_request->target_param_map[i]->key),\
        in_mess->ran_control_request->target_param_map[i]->value);
        ran_write(in_mess->ran_control_request->target_param_map[i]);
    }
}

const char* get_enum_name(RANParameter ran_par_enum){
    switch (ran_par_enum)
    {
    case RAN_PARAMETER__GNB_ID:
        return "gnb_id";
    case RAN_PARAMETER__SOMETHING:
        return "something";
    case RAN_PARAMETER__UE_LIST:
        return "ue_list";
    default:
        return "unrecognized param";
    }
}

void ran_write(RANParamMapEntry* target_param_map_entry){
    switch (target_param_map_entry->key)
    {
    case RAN_PARAMETER__GNB_ID:
        gnb_id = atoi(target_param_map_entry->value);
        break;
    case RAN_PARAMETER__SOMETHING:
        something = atoi(target_param_map_entry->value);
        break;
    default:
        printf("ERROR: cannot write RAN, unrecognized target param %d\n", target_param_map_entry->key);
    }
}

char* ran_read(RANParameter ran_par_enum){
    switch (ran_par_enum)
    {
    case RAN_PARAMETER__GNB_ID:
        return itoa(gnb_id);
    case RAN_PARAMETER__SOMETHING:
        return itoa(something);
    case RAN_PARAMETER__UE_LIST:
	return "wip";
    default:
        printf("unrecognized param %d\n",ran_par_enum);
        assert(0!=0);
    }
}

char* itoa(int i){
    int length = (snprintf(NULL, 0,"%d",i)+1);
    char* ret = malloc(length*sizeof(char));
    sprintf(ret, "%d", i);
    return ret;
}

