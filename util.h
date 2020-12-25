#ifndef MEMPOOL_UTIL_H
#define MEMPOOL_UTIL_H

#include <stdlib.h>

#ifdef __MINGW32__
#include <time.h>
#else
#include "taskLib.h"
#endif

#include "statusdef.h"

/* 生成随机整数 */
uint rand_uint(const uint m);

/* 等待若干毫秒 */
int msleep(long miliseconds);

#endif //MEMPOOL_UTIL_H
