#include "lock.h"

/* 初始化锁 */
error lock_init(Lock *const self) {
    lock_log("init");

#ifdef __MINGW32__
    if (pthread_mutex_init(&self->mutex, NULL) != 0) {
        serror("err");
        return Error;
    }
#else
        self->sem = semBCreate(SEM_Q_FIFO, SEM_FULL);
#endif

    lock_log("ok");
    return Success;
}

/* 获取锁 */
error lock_lock(Lock *const self) {
#ifdef __MINGW32__
    lock_log("lock=%d", pthread_mutex_lock(&self->mutex));
#else
    lock_log("");
    semTake(self->sem, WAIT_FOREVER);
#endif

    return Success;
}

/* 释放锁 */
error lock_unlock(Lock *const self) {
#ifdef __MINGW32__
    lock_log("unlock=%d", pthread_mutex_unlock(&self->mutex));
#else
    lock_log("");
    semGive(self->sem);
#endif

    return Success;
}

/* 获取并释放锁 */
error lock_reunlock(Lock *const self) {
    error err;
    err = lock_lock(self);
    if (err != Success) {
        return err;
    }

    err = lock_unlock(self);
    return err;
}

/* 销毁锁 */
error lock_delete(Lock *const self) {
#ifdef __MINGW32__
    lock_log("del=%d", pthread_mutex_destroy(&self->mutex));
#else
    lock_log("");
    semDelete(self->sem);
#endif

    return Success;
}
