#include "xmproducer.h"
#include "xmtext.h"

error xm_producer_init(xm_producer *self, xm_channel *channel) {
    self->channel = channel;

    return Success;
}

error xm_producer_run(xm_producer *self) {
    xm_channel_send_data(self->channel, XM_TEXT_LENGTH, XM_TEXT);
    return Success;
}
