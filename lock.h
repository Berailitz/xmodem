#ifndef MEMPOOL_LOCK_H
#define MEMPOOL_LOCK_H

#include "string.h"
#include "statusdef.h"
#include "log.h"

#ifdef __MINGW32__

#include <pthread.h>

#else
#include "semLib.h"
#endif

/* 锁 */
typedef struct {
#ifdef __MINGW32__
    pthread_mutex_t mutex;
#else
    SEM_ID sem;
#endif
} Lock;

/* 日志级别 */
#define lock_log strace

/* 初始化锁 */
error lock_init(Lock *const self);

/* 获取锁 */
error lock_lock(Lock *const self);

/* 释放锁 */
error lock_unlock(Lock *const self);

/* 销毁锁 */
error lock_delete(Lock *const self);

#endif //MEMPOOL_LOCK_H
