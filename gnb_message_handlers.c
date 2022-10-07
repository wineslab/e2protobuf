#include "gnb_message_handlers.h"
#include "oai-oran-protolib/ran_messages.pb-c.h"
#include <assert.h>
#include <stdio.h>

void handle_subscription(RANMessage* in_mess){
    printf("Not implemented\n");
    assert(0!=0);
}
void handle_indication_request(RANMessage* in_mess){
    printf("Indication request for %lu parameters:\n", in_mess->ran_indication_request->n_target_params);
    for(int par_i=0; par_i<in_mess->ran_indication_request->n_target_params; par_i++){
        printf("\tParameter id %d requested (a.k.a %s)\n",\
        in_mess->ran_indication_request->target_params[par_i],\
        get_enum_name(in_mess->ran_indication_request->target_params[par_i]));
    }
}
void handle_indication_response(RANMessage* in_mess){
    printf("Not implemented\n");
    assert(0!=0);
}
void handle_control(RANMessage* in_mess){
    printf("Not implemented\n");
    assert(0!=0);
}

const char* get_enum_name(RANParameter ran_par_enum){
    switch (ran_par_enum)
    {
    case RAN_PARAMETER__GNB_ID:
        return "gnb_id";
    case RAN_PARAMETER__SOMETHING:
     return "something";
    default:
        return "unrecognized param";
    }
}

