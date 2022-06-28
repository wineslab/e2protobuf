//
// Created by Eugenio Moro on 24/06/22.
//

#include "e2prtbf_common.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <printf.h>

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
    int fd = (intptr_t)stream->state;
    return send(fd, buf, count, 0) == count;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
    int fd = (intptr_t)stream->state;
    int result;

    if (count == 0)
        return true;

    result = recv(fd, buf, count, MSG_WAITALL);
    printf("Received %d bytes, count was %lu\n", result,count);

    if (result == 0)
        stream->bytes_left = 0; /* EOF */

    return result == count;
}

pb_ostream_t pb_ostream_from_socket(int fd)
{
    pb_ostream_t stream = {&write_callback, (void*)(intptr_t)fd, SIZE_MAX, 0};
    return stream;
}

pb_istream_t pb_istream_from_socket(int fd)
{
    pb_istream_t stream = {&read_callback, (void*)(intptr_t)fd, SIZE_MAX};
    return stream;
}