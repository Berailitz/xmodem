#ifndef TIMER_UINTERRUPT_H
#define TIMER_UINTERRUPT_H

#include "concurrent.h"
#include "statusdef.h"
#include "lock.h"
#include "log.h"
#include "util.h"


#ifndef __MINGW32__
#include <tickLib.h>
#endif

/* 用户中断 */
typedef struct {
    State state;
    uint interval_ms;
    Lock lock;

    void (*func)();
} UIS;

/* 中断任务回调 */
void *uinterrupt_async_callback(void *_);

/* 中断任务 */
void *uinterrupt_task(void *_);

/* 初始化 */
error uinterrupt_init();

/* 中断响应函数 */
#ifndef __MINGW32__
void uinterrupt_on_tick();
#endif

/* 启动 */
error uinterrupt_start(void (*func)(), const uint interval_ms);

/* 停止 */
error uinterrupt_stop();

/* 销毁资源 */
error uinterrupt_clear();

#endif //TIMER_UINTERRUPT_H
