#ifndef XMPRODUCER_H
#define XMPRODUCER_H

#include "xmqueue.h"

typedef struct {
    xm_channel *channel;
} xm_producer;

error xm_producer_init(xm_producer *self, xm_channel *channel);

error xm_producer_run(xm_producer *self);

#endif //XMPRODUCER_H
