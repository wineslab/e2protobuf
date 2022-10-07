#include "oai-oran-protolib/ran_messages.pb-c.h"

void handle_subscription(RANMessage* in_mess);
void handle_indication_request(RANMessage* in_mess);
void handle_indication_response(RANMessage* in_mess);
void handle_control(RANMessage* in_mess);
const char* get_enum_name(RANParameter ran_par_enum);