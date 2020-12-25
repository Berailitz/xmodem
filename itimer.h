#ifndef TIMER_ITIMER_H
#define TIMER_ITIMER_H

#include "lock.h"
#include "uinterrupt.h"

/* 最大计时器数量 */
#define MAX_TIMER_NUM 100

/* 计时器周期 */
const uint INTERRUPT_INTERVAL_MS;

/* 计时器 */
typedef struct {
    uint next;
    uint interval;
    callback_func func;
    callback_param param;
} Timer;

/* 计时器池 */
typedef struct {
    Lock timer_lock;
    Lock tick_lock;
    State state;
    Timer timers[MAX_TIMER_NUM];
    Executor task;
    uint tick;
} TimerPool;

/* 不存在的计时器id */
const uint TIMER_INVALID_ID;

/* 计时器池实例 */
TimerPool tpool;

/* 中断后的任务 */
void tpool_on_tick();

/* 获取当前tick */
uint tpool_get_current_tick();

/* 计时器池任务 */
void *tpool_task(void *_);

/* 初始化 */
error tpool_init();

/* 启动 */
error tpool_start();

/* 查找空闲的计时器 */
uint tpool_find_available_timer();

/* 注册计时器 */
uint tpool_register(const callback_func callback, callback_param param, const uint timeout, const uint interval);

/* 取消注册 */
error tpool_unregister(const uint idx);

/* 暂停 */
error tpool_stop();

/* 销毁计时器池 */
error tpool_clear();

#endif //TIMER_ITIMER_H
