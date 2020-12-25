#ifndef MEMPOOL_STATUSDEF_H
#define MEMPOOL_STATUSDEF_H

/* 函数执行状态码 */
typedef unsigned int uint;
typedef enum {
    Success = 0,
    Error = 1
} error;

/* 结构的状态码 */
typedef enum {
    Invalid, Valid
} State;

#endif