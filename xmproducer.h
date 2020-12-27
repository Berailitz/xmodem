#ifndef XMPRODUCER_H
#define XMPRODUCER_H

#include "xmqueue.h"

/* 发送方 */
typedef struct {
    /* 传输通道 */
    xm_channel *channel;
} xm_producer;

/* 初始化函数 */
error xm_producer_init(xm_producer *self, xm_channel *channel);

/* 启动函数 */
error xm_producer_run(xm_producer *self);

#endif //XMPRODUCER_H
