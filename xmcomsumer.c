#include "xmcomsumer.h"

const uint XM_MAX_NAK_COUNTER = 3;

error xm_consumer_init(xm_consumer *self, xm_channel *channel) {
    self->channel = channel;
    self->nak_timer = 0;
    self->nak_counter = 0;
    lock_init(&self->exit);
    lock_lock(&self->exit);

    xm_channel_register_receiver(self->channel, xm_consumer_receive, self);

    return Success;
}

error xm_consumer_start(xm_consumer *self) {
    self->nak_timer = tpool_register(xm_consumer_nak_tick, self, 1, 0);

    return Success;
}

void *xm_consumer_nak_tick(void *arg) {
    xm_consumer *self = arg;
    error err;

    self->nak_timer = 0;

    err = xm_channel_send_flag(self->channel, XM_FLAG_NAK);
    if (err != Success) {
        lock_unlock(&self->exit);
        return NULL;
    }

    self->nak_counter++;
    if (self->nak_counter > XM_MAX_NAK_COUNTER) {
        lock_unlock(&self->exit);
    }

    self->nak_timer = tpool_register(xm_consumer_nak_tick, self, 1, 0);

    return NULL;
}

void *xm_consumer_receive(void *arg, const uint length, const byte *data) {
    xm_consumer *self = arg;

    sinfo("receive");
    tpool_unregister(self->nak_timer);

    sinfo("%.*s", length, data);

    return NULL;
}

error xm_consumer_clear(xm_consumer *self) {
    tpool_unregister(self->nak_timer);
    lock_delete(&self->exit);
    return Success;
}
