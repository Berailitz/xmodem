#include "xmframe.h"
#include "string.h"
#include "log.h"

const byte XM_FRAME_MAX_LENGTH = 128;

error xm_frame_dump(xm_frame *self, byte* data) {
    memcpy(xm_frame_load(self), data, self->length);

    return Success;
}

byte* xm_frame_load(xm_frame *self) {
    return (byte*)self + sizeof(xm_frame);
}

error xm_frame_gen_sum(xm_frame *self) {
    self->sum = xm_frame_calculate_sum(self);

    return Success;
}

byte xm_frame_calculate_sum(xm_frame *self) {
    byte *data = xm_frame_load(self);
    byte sum = 0;
    uint i;
    for (i = 0; i < self->length; i++) {
        sum += *(data + i);
    }

    return sum;
}

error xm_frame_check_sum(xm_frame *self) {
    uint sum = xm_frame_calculate_sum(self);

    sdebug("expected=%d received=%d", sum, self->sum);
    if (sum == self->sum) {
        return Success;
    }

    return Error;
}
