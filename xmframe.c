#include "xmframe.h"
#include "string.h"
#include "log.h"

const byte XM_FRAME_MAX_LENGTH = 128;

xm_flag xm_frame_get_flag(xm_frame_header *self) {
    return self->flag.flag;
}

error xm_frame_set_flag(xm_frame_header *self, xm_flag flag) {
    self->flag.flag = flag;
    return Success;
}

error xm_frame_dump(xm_frame_header *self, byte* data) {
    memcpy(xm_frame_load(self), data, self->length);

    return Success;
}

byte* xm_frame_load(xm_frame_header *self) {
    return (byte*)self + sizeof(xm_frame_header);
}

error xm_frame_gen_sum(xm_frame_header *self) {
    *xm_frame_get_sum(self) = xm_frame_calculate_sum(self);

    return Success;
}

byte *xm_frame_get_sum(xm_frame_header *self) {
    return xm_frame_load(self) + self->length;
}

byte xm_frame_calculate_sum(xm_frame_header *self) {
    byte *data = xm_frame_load(self);
    byte sum = 0;
    uint i;
    for (i = 0; i < self->length; i++) {
        sum += *(data + i);
    }

    return sum;
}

error xm_frame_check_sum(xm_frame_header *self) {
    uint expected = xm_frame_calculate_sum(self);
    uint received = *xm_frame_get_sum(self);

    sdebug("expected=%d received=%d", expected, received);
    if (expected == received) {
        return Success;
    }

    return Error;
}
