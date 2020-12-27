#ifndef XMMODEM_H
#define XMMODEM_H

#include "xmproducer.h"
#include "xmcomsumer.h"
#include "test.h"

/* xmodem结构 */
typedef struct {
    /* 传输端点对 */
    xm_channel_pair pair;
    /* 发送方 */
    xm_producer producer;
    /* 接受方 */
    xm_consumer consumer;
} xmodem;

/* 初始化函数 */
error xmodem_init(xmodem *self);

/* 销毁函数 */
error xmodem_clear(xmodem *self);

/* 启动传输 */
error xmodem_run(xmodem *self);

#endif //XMMODEM_H
