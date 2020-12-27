#ifndef XMQUEUE_H
#define XMQUEUE_H

#include "allocator.h"
#include "xmframe.h"

/* 回调函数 */
typedef void *(*xm_channel_receiver)(void *self, const uint length, const byte *data);

/* 队列 */
typedef struct {
    /* 下一帧序号 */
    byte next;
    /* 同步信号量 */
    Lock lock;
} xm_queue;

/* 最大重试次数 */
const uint XM_MAX_RESEND_COUNTER;

/* 收发端点 */
typedef struct xm_channel {
    /* 发送队列 */
    xm_queue send_queue;
    /* 接收队列 */
    xm_queue receive_queue;

    /* 内存分配器 */
    Allocator *allocator;

    /* 回调参数 */
    void *arg;
    /* 回调函数 */
    xm_channel_receiver callback;

    /* 发送缓冲区 */
    xm_frame_header *sent;
    /* 重发计数器 */
    uint resend_counter;

    /* 错误缓冲区 */
    error err;

    /* 对端 */
    struct xm_channel *counterpart;
} xm_channel;

/* 收发端点对 */
typedef struct {
    xm_channel producer;
    xm_channel consumer;
} xm_channel_pair;

/* 数据包 */
typedef struct {
    xm_channel *channel;
    xm_frame_header *frame;
} xm_package;

/* 初始化队列 */
error xm_queue_init(xm_queue *self);

/* 销毁队列 */
error xm_queue_clear(xm_queue *self);

/* 初始化端点 */
error xm_channel_init(xm_channel *self);

/* 销毁端点 */
error xm_channel_clear(xm_channel *self);

/* 初始化端点对 */
error xm_channel_pair_init(xm_channel_pair *self);

/* 销毁端点对 */
error xm_channel_pair_clear(xm_channel_pair *self);

/* 注册接收回调 */
error xm_channel_register_receiver(xm_channel *self, xm_channel_receiver receiver, void *arg);

/* 发送数据 */
error xm_channel_send_data(xm_channel *self, uint size, const char *data);

/* 发送标志位 */
error xm_channel_send_flag(xm_channel *self, xm_flag flag);

/* 发送帧 */
error xm_channel_send_frame(xm_channel *self, xm_frame_header *frame);

/* 接收数据包 */
void *xm_package_receive(void *arg);

/* 重发回调函数 */
void *xm_package_resend_tick(void *arg);

/* 重发函数 */
void *xm_package_resend(xm_package *self);

#endif //XMQUEUE_H
