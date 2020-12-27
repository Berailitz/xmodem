#ifndef XMMODEM_H
#define XMMODEM_H

#include "xmproducer.h"
#include "xmcomsumer.h"
#include "test.h"

typedef struct {
    xm_channel_pair pair;
    xm_producer producer;
    xm_consumer consumer;
} xmodem;

error xmodem_init(xmodem *self);

error xmodem_run(xmodem *self);

#define xm_test_base(name, test_block) \
error xm_test_##name() {               \
    xmodem self; \
 \
    plog_sep(level, "TEST "#name" START"); \
 \
    xmodem_init(&self); \
 \
    tpool_start(); \
 \
    test_block \
 \
    tpool_stop(); \
 \
    plog_sep(level, "TEST "#name" END"); \
 \
    return Success; \
}

#endif //XMMODEM_H
