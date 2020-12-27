#include "xmproducer.h"
#include "xmtext.h"

/* 初始化函数 */
error xm_producer_init(xm_producer *self, xm_channel *channel) {
    self->channel = channel;

    return Success;
}

/* 启动函数 */
error xm_producer_run(xm_producer *self) {
    xm_channel_send_data(self->channel, XM_TEXT_LENGTH, XM_TEXT);
    return Success;
}
