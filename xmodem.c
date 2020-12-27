#include "xmodem.h"

/* 初始化函数 */
error xmodem_init(xmodem *self) {
    xm_channel_pair_init(&self->pair);
    xm_producer_init(&self->producer, &self->pair.producer);
    xm_consumer_init(&self->consumer, &self->pair.consumer);
    return Success;
}

/* 销毁函数 */
error xmodem_clear(xmodem *self) {
    xm_channel_pair_clear(&self->pair);
    xm_consumer_clear(&self->consumer);

    return Success;
}

/* 启动传输 */
error xmodem_run(xmodem *self) {
    xm_consumer_start(&self->consumer);
    xm_producer_run(&self->producer);
    return Success;
}
