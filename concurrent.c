#include "concurrent.h"

/* 异步执行任务 */
error executor_async(callback_func func, callback_param arg) {
#ifdef __MINGW32__
    pthread_t task;
    pinfo("%d", pthread_create(&task, NULL, func, arg));
    pthread_detach(task);
#else
    pinfo("%d", taskSpawn("", 0, 0, 1024, func, arg, 0, 0, 0, 0, 0, 0, 0, 0, 0));
#endif

    return Success;
}

/* 执行任务 */
error executor_start(Executor *const self, callback_func func, callback_param arg) {
#ifdef __MINGW32__
    sinfo("%d", pthread_create(&self->task, NULL, func, arg));
#else
    sinfo("%d", taskSpawn("", 0, 0, 1024, func, arg, 0, 0, 0, 0, 0, 0, 0, 0, 0));
#endif

    return Success;
}

/* 阻塞任务 */
error executor_join(Executor *const self) {
#ifdef __MINGW32__
    sinfo("%d", pthread_join(self->task, NULL));
#endif
    return Success;
}
