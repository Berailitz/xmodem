#ifndef XMQUEUE_H
#define XMQUEUE_H

#include "allocator.h"
#include "xmframe.h"

/* 回调函数 */
typedef void *(*xm_channel_receiver)(void *self, const uint length, const byte *data);

typedef struct {
    byte next;
    Lock lock;
} xm_queue;

const uint XM_MAX_RESEND_COUNTER;

typedef struct xm_channel {
    xm_queue send_queue;
    xm_queue receive_queue;

    Allocator *allocator;

    void *arg;
    xm_channel_receiver callback;

    xm_frame_header *sent;
    uint resend_counter;

    error err;

    struct xm_channel *counterpart;
} xm_channel;

typedef struct {
    xm_channel producer;
    xm_channel consumer;
} xm_channel_pair;

typedef struct {
    xm_channel *channel;
    xm_frame_header *frame;
} xm_package;

error xm_queue_init(xm_queue *self);

error xm_channel_init(xm_channel *self);

error xm_channel_pair_init(xm_channel_pair *self);

error xm_channel_register_receiver(xm_channel *self, xm_channel_receiver receiver, void *arg);

error xm_channel_send_data(xm_channel *self, uint size, const char *data);

error xm_channel_send_flag(xm_channel *self, xm_flag flag);

error xm_channel_send_frame(xm_channel *self, xm_frame_header *frame);

void *xm_package_receive(void *arg);

void *xm_package_resend_tick(void *arg);

void *xm_package_resend(xm_package *self);

#endif //XMQUEUE_H
