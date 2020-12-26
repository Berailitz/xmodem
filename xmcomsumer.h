#ifndef XMCOMSUMER_H
#define XMCOMSUMER_H

#include "xmqueue.h"
#include "itimer.h"

typedef struct {
    xm_channel *channel;
    uint nak_timer;
} xm_consumer;

error xm_consumer_init(xm_consumer *self, xm_channel *channel);

error xm_consumer_start(xm_consumer *self);

void *xm_consumer_nak_tick(void *arg);

void *xm_consumer_receive(void *arg, const uint length, const byte *data);

#endif //XMCOMSUMER_H
