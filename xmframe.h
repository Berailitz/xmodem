#ifndef XMODEM_XMFRAME_H
#define XMODEM_XMFRAME_H

#include "statusdef.h"

typedef unsigned char byte;

typedef byte xm_flag;

#define XM_FLAG_SOH 0
#define XM_FLAG_EOT 0x04
#define XM_FLAG_ACK 0x06
#define XM_FLAG_NAK 0x15
#define XM_FLAG_CAN 0x18

const byte XM_FRAME_MAX_LENGTH;

typedef struct {
    xm_flag flag;
} xm_frame_flag;

typedef struct {
    xm_frame_flag flag;
    byte id;
    byte length;
} xm_frame_header;

xm_flag xm_frame_get_flag(xm_frame_header *self);

error xm_frame_set_flag(xm_frame_header *self, xm_flag flag);

error xm_frame_dump(xm_frame_header *self, byte* data);

byte* xm_frame_load(xm_frame_header *self);

error xm_frame_gen_sum(xm_frame_header *self);

byte *xm_frame_get_sum(xm_frame_header *self);

byte xm_frame_calculate_sum(xm_frame_header *self);

error xm_frame_check_sum(xm_frame_header *self);

#endif //XMODEM_XMFRAME_H
