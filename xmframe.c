#include "xmframe.h"
#include "string.h"
#include "log.h"

/* 最大数据长度 */
const byte XM_FRAME_MAX_LENGTH = 128;

/* 查询标志位 */
xm_flag xm_frame_get_flag(xm_frame_header *self) {
    return self->flag.flag;
}

/* 设置标志位 */
error xm_frame_set_flag(xm_frame_header *self, xm_flag flag) {
    self->flag.flag = flag;
    return Success;
}

/* 保存数据 */
error xm_frame_dump(xm_frame_header *self, byte* data) {
    memcpy(xm_frame_load(self), data, self->length);

    return Success;
}

/* 读取数据 */
byte* xm_frame_load(xm_frame_header *self) {
    return (byte*)self + sizeof(xm_frame_header);
}

/* 生成校验和 */
error xm_frame_gen_sum(xm_frame_header *self) {
    *xm_frame_get_sum(self) = xm_frame_calculate_sum(self);

    return Success;
}

/* 读取校验和 */
byte *xm_frame_get_sum(xm_frame_header *self) {
    return xm_frame_load(self) + self->length;
}

/* 计算校验和 */
byte xm_frame_calculate_sum(xm_frame_header *self) {
    byte *data = xm_frame_load(self);
    byte sum = 0;
    uint i;
    for (i = 0; i < self->length; i++) {
        sum += *(data + i);
    }

    return sum;
}

/* 检查校验和 */
error xm_frame_check_sum(xm_frame_header *self) {
    uint expected = xm_frame_calculate_sum(self);
    uint received = *xm_frame_get_sum(self);

    sdebug("expected=%d received=%d", expected, received);
    if (expected == received) {
        return Success;
    }

    return Error;
}
