#ifndef XMMODEM_H
#define XMMODEM_H

#include "xmproducer.h"
#include "xmcomsumer.h"

typedef struct {
    xm_channel_pair pair;
    xm_producer producer;
    xm_consumer consumer;
} xmodem;

error xmodem_init(xmodem *self);

error xmodem_run(xmodem *self);

#endif //XMMODEM_H
