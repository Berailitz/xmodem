#include "uinterrupt.h"

/* 用户中断实例 */
UIS uis;

/* 中断任务回调 */
void *uinterrupt_async_callback(void *_) {
    (*uis.func)();
    return NULL;
}

/* 中断任务 */
void *uinterrupt_task(void *_) {
    while (uis.state == Valid) {
#ifdef __MINGW32__
        msleep(uis.interval_ms);
        lock_lock(&uis.lock);
        lock_unlock(&uis.lock);
#else
        lock_lock(&uis.lock);
#endif

        executor_async(uinterrupt_async_callback, NULL);
    }

    return NULL;
}

/* 初始化 */
error uinterrupt_init() {
    if (uis.state == Valid) {
        pdebug("already init");
    } else {
        lock_init(&uis.lock);
        lock_lock(&uis.lock);
        uis.state = Valid;
        executor_async(uinterrupt_task, NULL);
#ifndef __MINGW32__
        sysClkDisable();
#endif
    }

    return Success;
}

/* 中断响应函数 */
#ifndef __MINGW32__
void uinterrupt_on_tick() {
    tickAnnounce();
    lock_unlock(&uis.lock);
}
#endif

/* 启动 */
error uinterrupt_start(void (*func)(), const uint interval_ms) {
    pinfo("func=%p interval_ms=%d", func, interval_ms);
    uis.func = func;
    uis.interval_ms = interval_ms;

#ifdef __MINGW32__
    lock_unlock(&uis.lock);
#else
    sysClkConnect((FUNCPTR)uinterrupt_on_tick, 0);
    sysClkRateSet(1000/interval_ms);
    sysClkEnable();
#endif

    return Success;
}

/* 停止 */
error uinterrupt_stop() {
    pdebug("");

#ifdef __MINGW32__
    lock_lock(&uis.lock);
#else
    sysClkDisable();
#endif

    return Success;
}

/* 销毁资源 */
error uinterrupt_clear() {
    pdebug("");
#ifdef __MINGW32__
    uis.state = Invalid;
#endif
    return Success;
}
