#ifndef MEMPOOL_CONCURRENT_H
#define MEMPOOL_CONCURRENT_H

#include "string.h"
#include "statusdef.h"
#include "log.h"

#ifdef __MINGW32__
#include <pthread.h>
#else
#include <taskLib.h>
#endif

/* 任务 */
typedef struct {
#ifdef __MINGW32__
    pthread_t task;
#endif
} Executor;

/* 回调参数 */
typedef void *callback_param;

/* 回调函数 */
typedef void *(*callback_func)(callback_param);

/* 异步执行任务 */
error executor_async(callback_func func, callback_param arg);

/* 执行任务 */
error executor_start(Executor *const self, callback_func func, callback_param arg);

/* 阻塞任务 */
error executor_join(Executor *const self);

#endif //MEMPOOL_CONCURRENT_H
