#include "itimer.h"

/* 计时器周期 */
const uint INTERRUPT_INTERVAL_MS = 1000;

/* 不存在的计时器id */
const uint TIMER_INVALID_ID = MAX_TIMER_NUM;

/* 计时器池实例 */
TimerPool tpool;

/* 中断后的任务 */
void tpool_on_tick() {
    pdebug("");

    tpool.tick++;
    lock_unlock(&tpool.tick_lock);
}

/* 获取当前tick */
uint tpool_get_current_tick() {
    return tpool.tick;
}

/* 计时器池任务 */
void *tpool_task(void *_) {
    while (tpool.state == Valid) {
        uint i;
        uint tick;

        lock_lock(&tpool.tick_lock);

        tick = tpool_get_current_tick();
        lock_lock(&tpool.timer_lock);

        for (i = 0; i < MAX_TIMER_NUM; i++) {
            Timer *current = &tpool.timers[i];
            if (current->next == 0 || current->next > tick) {
                continue;
            }

            (*current->func)(current->param);

            current->next = 0;
            if (current->interval > 0) {
                current->next = tick + current->interval;
            } else {
                current->func = NULL;
                current->param = NULL;
            }
        }

        lock_unlock(&tpool.timer_lock);
    }

    return NULL;
}

/* 初始化 */
error tpool_init() {
    pdebug("");
    lock_init(&tpool.timer_lock);
    lock_init(&tpool.tick_lock);
    lock_lock(&tpool.tick_lock);
    tpool.state = Invalid;
    tpool.tick = 0;
    memset((void *) tpool.timers, 0, MAX_TIMER_NUM * sizeof(Timer));
    uinterrupt_init();
    return Success;
}

/* 启动 */
error tpool_start() {
    pdebug("");
    tpool.state = Valid;
    executor_start(&tpool.task, tpool_task, NULL);
    uinterrupt_start(tpool_on_tick, INTERRUPT_INTERVAL_MS);
    return Success;
}

/* 查找空闲的计时器 */
uint tpool_find_available_timer() {
    uint i;
    for (i = 0; i < MAX_TIMER_NUM; i++) {
        Timer *current = &tpool.timers[i];
        if (current->next == 0) {
            pdebug("i=%d", i);
            return i;
        }
    }

    pinfo("full");
    return TIMER_INVALID_ID;
}

/* 注册计时器 */
uint tpool_register(const callback_func callback, callback_param param, const uint timeout, const uint interval) {
    Timer *timer;
    uint timer_idx;

    pdebug("callback=%p param=%d timeout=%d interval=%d", callback, param, timeout, interval);

    lock_lock(&tpool.timer_lock);
    timer_idx = tpool_find_available_timer();
    if (timer_idx == TIMER_INVALID_ID) {
        pdebug("full");
        lock_unlock(&tpool.timer_lock);
        return 0;
    }

    timer = &tpool.timers[timer_idx];
#ifdef __MINGW32__
    timer->next = tpool_get_current_tick() + timeout;
    timer->interval = interval;
#else
    timer->next = tpool_get_current_tick() + timeout * 10;
    timer->interval = interval * 10;
#endif
    timer->func = callback;
    timer->param = param;
    lock_unlock(&tpool.timer_lock);
    return timer_idx;
}

/* 取消注册 */
error tpool_unregister(const uint idx) {
    Timer *current;

    pdebug("idx=%d", idx);
    if (idx < 0 || idx >= MAX_TIMER_NUM) {
        perror("invalid");
        return Error;
    }

    current = &tpool.timers[idx];
    lock_lock(&tpool.timer_lock);
    if (current->next == 0) {
        pinfo("empty");
        lock_unlock(&tpool.timer_lock);
        return Success;
    }

    current->next = 0;
    current->interval = 0;
    current->func = NULL;
    current->param = 0;
    return Success;
}

/* 暂停 */
error tpool_stop() {
    pdebug("");
    uinterrupt_stop();
}

/* 销毁计时器池 */
error tpool_clear() {
    pdebug("");
    tpool.state = Invalid;
    uinterrupt_clear();
    lock_delete(&tpool.timer_lock);
    lock_delete(&tpool.tick_lock);
    return Success;
}
