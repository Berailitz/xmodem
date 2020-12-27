#include "xmodem.h"

error xmodem_init(xmodem *self) {
    xm_channel_pair_init(&self->pair);
    xm_producer_init(&self->producer, &self->pair.producer);
    xm_consumer_init(&self->consumer, &self->pair.consumer);
    return Success;
}

error xmodem_clear(xmodem *self) {
    xm_channel_pair_clear(&self->pair);
    xm_consumer_clear(&self->consumer);

    return Success;
}

error xmodem_run(xmodem *self) {
    xm_consumer_start(&self->consumer);
    xm_producer_run(&self->producer);
    return Success;
}
