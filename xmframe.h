#ifndef XMODEM_XMFRAME_H
#define XMODEM_XMFRAME_H

#include "statusdef.h"

/* 定义字节 */
typedef unsigned char byte;

/* 定义标志位类型 */
typedef byte xm_flag;

/* 开始传输数据标志位 */
#define XM_FLAG_SOH 0
/* 结束传输标志位 */
#define XM_FLAG_EOT 0x04
/* 收到标志位 */
#define XM_FLAG_ACK 0x06
/* 重发标志位 */
#define XM_FLAG_NAK 0x15
/* 取消标志位 */
#define XM_FLAG_CAN 0x18

/* 最大数据长度 */
const byte XM_FRAME_MAX_LENGTH;

/* 标志位 */
typedef struct {
    xm_flag flag;
} xm_frame_flag;

/* 帧头 */
typedef struct {
    /* 标志位 */
    xm_frame_flag flag;
    /* 帧序号 */
    byte id;
    /* 数据长度 */
    byte length;
} xm_frame_header;

/* 查询标志位 */
xm_flag xm_frame_get_flag(xm_frame_header *self);

/* 设置标志位 */
error xm_frame_set_flag(xm_frame_header *self, xm_flag flag);

/* 保存数据 */
error xm_frame_dump(xm_frame_header *self, byte* data);

/* 读取数据 */
byte* xm_frame_load(xm_frame_header *self);

/* 生成校验和 */
error xm_frame_gen_sum(xm_frame_header *self);

/* 读取校验和 */
byte *xm_frame_get_sum(xm_frame_header *self);

/* 计算校验和 */
byte xm_frame_calculate_sum(xm_frame_header *self);

/* 检查校验和 */
error xm_frame_check_sum(xm_frame_header *self);

#endif //XMODEM_XMFRAME_H
