#ifndef XMCOMSUMER_H
#define XMCOMSUMER_H

#include "xmqueue.h"
#include "itimer.h"

/* 最多发送多少个nak即停止 */
const uint XM_MAX_NAK_COUNTER;

/* 接收方 */
typedef struct {
    /* 链接 */
    xm_channel *channel;

    /* nak定时器 */
    uint nak_timer;

    /* nak计数器 */
    uint nak_counter;

    /* 停止信号量 */
    Lock exit;
} xm_consumer;

/* 初始化接收方 */
error xm_consumer_init(xm_consumer *self, xm_channel *channel);

/* 启动接收方 */
error xm_consumer_start(xm_consumer *self);

/* 超时发送nak的回调函数 */
void *xm_consumer_nak_tick(void *arg);

/* 接收方接受数据的回调函数 */
void *xm_consumer_receive(void *arg, const uint length, const byte *data);

/* 销毁接收方 */
error xm_consumer_clear(xm_consumer *self);

#endif //XMCOMSUMER_H
