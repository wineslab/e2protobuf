//
// Created by Eugenio Moro on 24/06/22.
//

#include "gnb_connector.h"

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


/* Handle one arriving client connection.
 * Clients are expected to send a ListFilesRequest, terminated by a '0'.
 * Server will respond with a ListFilesResponse message.
 */

void build_dummy_response(E2_REQID_t req_id, E2_dummy_response* rsp){
    rsp->req_id = 1;
    rsp->req_id = req_id;
    strcpy(rsp->mess_string,"dummy response");
    rsp->result = true;
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
            build_dummy_response(E2_REQID_DUMMY1,rsp);
        }
        case E2_REQID_DUMMY2:
            break;
        default:
            printf("Unrecognized request id %d\n",request.id);
    }
    ListFilesResponse response = {};
    pb_ostream_t output = pb_ostream_from_socket(connfd);

    if (directory == NULL) {
        perror("opendir");

        /* Directory was not found, transmit error status */
        response.has_path_error = true;
        response.path_error = true;
    } else {
        /* Directory was found, transmit filenames */
        response.has_path_error = false;
        response.file = directory;
    }

    if (!pb_encode_delimited(&output, ListFilesResponse_fields, &response)) {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&output));
    }


    if (directory != NULL)
        closedir(directory);
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    int reuse = 1;

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

    for (;;) {
        /* Wait for a client */
        connfd = accept(listenfd, NULL, NULL);

        if (connfd < 0) {
            perror("accept");
            return 1;
        }

        printf("Got connection.\n");

        handle_connection(connfd);

        printf("Closing connection.\n");

        close(connfd);
    }

    return 0;
}