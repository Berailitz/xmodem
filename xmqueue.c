#include "xmqueue.h"
#include "concurrent.h"
#include "itimer.h"

/* 最大重试次数 */
const uint XM_MAX_RESEND_COUNTER = 3;

/* 初始化队列 */
error xm_queue_init(xm_queue *self) {
    self->next = 0;
    lock_init(&self->lock);
    return Success;
}

/* 销毁队列 */
error xm_queue_clear(xm_queue *self) {
    lock_delete(&self->lock);

    return Success;
}

/* 初始化端点 */
error xm_channel_init(xm_channel *self) {
    xm_queue_init(&self->send_queue);
    xm_queue_init(&self->receive_queue);

    self->allocator = allocator_new(0, 0);

    self->arg = NULL;
    self->callback = NULL;

    self->sent = NULL;
    self->resend_counter = 0;

    self->err = Success;

    self->counterpart = NULL;

    return Success;
}

/* 销毁端点 */
error xm_channel_clear(xm_channel *self) {
    allocator_delete(self->allocator);
    xm_queue_clear(&self->send_queue);
    xm_queue_clear(&self->receive_queue);

    return Success;
}

/* 初始化端点对 */
error xm_channel_pair_init(xm_channel_pair *self) {
    xm_channel_init(&self->producer);
    xm_channel_init(&self->consumer);

    self->consumer.counterpart = &self->producer;
    self->producer.counterpart = &self->consumer;

    return Success;
}

/* 销毁端点对 */
error xm_channel_pair_clear(xm_channel_pair *self) {
    xm_channel_clear(&self->consumer);
    xm_channel_clear(&self->producer);

    return Success;
}

/* 注册接收回调 */
error xm_channel_register_receiver(xm_channel *self, xm_channel_receiver receiver, void *arg) {
    self->arg = arg;
    self->callback = receiver;

    return Success;
}

/* 发送数据 */
error xm_channel_send_data(xm_channel *self, uint size, const char *data) {
    error err;
    uint sent;

    for (sent = 0; sent < size;) {
        xm_frame_header *frame;
        uint length = size - sent;
        if (length > XM_FRAME_MAX_LENGTH) {
            length = XM_FRAME_MAX_LENGTH;
        }

        frame = allocator_allocate(self->allocator, sizeof(xm_frame_header) + length + 1);
        frame->length = length;
        xm_frame_set_flag(frame, XM_FLAG_SOH);
        xm_frame_dump(frame, (byte *) data + sent);
        xm_frame_gen_sum(frame);

        sinfo("frame=%p %.*s", frame, length, data + sent);

        lock_lock(&self->send_queue.lock);

        self->sent = frame;
        err = xm_channel_send_frame(self, frame);
        if (err != Success) {
            return err;
        }

        sent += length;
    }

    lock_lock(&self->send_queue.lock);

    xm_channel_send_flag(self, XM_FLAG_EOT);

    lock_unlock(&self->send_queue.lock);

    return Success;
}

/* 发送标志位 */
error xm_channel_send_flag(xm_channel *self, xm_flag flag) {
    xm_frame_header *frame = allocator_allocate(self->allocator, sizeof(xm_frame_flag));
    xm_frame_set_flag(frame, flag);
    return xm_channel_send_frame(self, frame);
}

/* 发送帧 */
error xm_channel_send_frame(xm_channel *self, xm_frame_header *frame) {
    xm_package *package = allocator_allocate(self->allocator, sizeof(xm_package));
    sinfo("flag=%d", frame->flag);

    if (xm_frame_get_flag(frame) == XM_FLAG_SOH) {
        frame->id = self->send_queue.next;
    }

    package->frame = frame;
    package->channel = self->counterpart;

    self->resend_counter = 0;
    tpool_register(xm_package_resend_tick, (void *) package, 1, 0);

    sinfo("package=%p channel=%p frame=%p", package, package->channel, package->frame);
    xm_package_receive(package);

    return self->err;
}

/* 接收数据包 */
void *xm_package_receive(void *arg) {
    xm_package *self = arg;

    sinfo("channel=%p flag=%d id=%d", self->channel, self->frame->flag, self->frame->id);

    if (xm_frame_get_flag(self->frame) == XM_FLAG_SOH && xm_frame_check_sum(self->frame) != Success) {
        serror("mismatch");

        return NULL;
    } else {
        sinfo("match");
    }

    switch (xm_frame_get_flag(self->frame)) {
        case XM_FLAG_SOH:
            sinfo("SOH");
            xm_channel_send_flag(self->channel, XM_FLAG_ACK);

            if (self->channel->callback == NULL) {
                sinfo("no callback");
                return NULL;
            }

            sinfo("callback");
            self->channel->callback(self->channel->arg, self->frame->length, xm_frame_load(self->frame));

            break;
        case XM_FLAG_ACK:
            sinfo("ack id=%d", self->channel->send_queue.next);
            self->channel->send_queue.next++;
            self->channel->counterpart->sent = NULL;
            lock_unlock(&self->channel->send_queue.lock);

            break;
        case XM_FLAG_NAK:
            if (self->channel->sent != NULL) {
                xm_package_resend(self);
            }
            break;
        default:
            self->channel->err = Error;
            break;
    }

    allocator_deallocate(self->channel->counterpart->allocator, self->frame);
    self->frame = NULL;

    return NULL;
}

/* 重发回调函数 */
void *xm_package_resend_tick(void *arg) {
    xm_package *self = arg;

    return xm_package_resend(self);
}

/* 重发函数 */
void *xm_package_resend(xm_package *self) {
    if (self->frame != NULL && self->frame->id == self->channel->send_queue.next) {
        self->channel->resend_counter++;
        if (self->channel->resend_counter >= XM_MAX_RESEND_COUNTER) {
            sinfo("end");
            xm_channel_send_flag(self->channel->counterpart, XM_FLAG_CAN);
            self->channel->err = Error;
            return NULL;
        }

        sinfo("resend id=%d", self->frame->id);
        xm_package_receive(self);
    }

    return NULL;
}
