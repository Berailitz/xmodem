#ifndef TIMER_TEST_H
#define TIMER_TEST_H

#include "lock.h"
#include "lock.h"
#include "util.h"
#include "concurrent.h"
#include "itimer.h"
#include "led.h"
#include "test.h"
#include <unistd.h>

/* 定义一个测试函数 */
#define ttest_base(ttest_name, register_block, callback_block) \
Lock ttest_lock_##ttest_name;    \
 \
void *ttest_callback_##ttest_name(callback_param param) { \
    const uint iparam = (long long) param; \
    pinfo("iparam=%p", iparam); \
    led_incr(); \
    switch (iparam) { \
    callback_block \
    case 0: \
        pinfo("exit"); \
        lock_unlock(&ttest_lock_##ttest_name); \
    } \
     \
    return NULL; \
} \
 \
void ttest_##ttest_name() { \
    plog_sep(level, "TEST "#ttest_name" START"); \
    led_init(); \
    tpool_init(); \
    tpool_start(); \
 \
    lock_init(&ttest_lock_##ttest_name); \
    lock_lock(&ttest_lock_##ttest_name); \
 \
 register_block \
 \
    lock_lock(&ttest_lock_##ttest_name); \
    lock_unlock(&ttest_lock_##ttest_name); \
    lock_delete(&ttest_lock_##ttest_name); \
 \
    tpool_stop(); \
    tpool_clear(); \
    plog_sep(level, "TEST "#ttest_name" END"); \
}

#define ttest_case(value) \
case value: \
    pinfo("case "#value); \
break;

/* 更便捷的测试函数宏 */
#define ttest(name, block) \
ttest_base(name, block, {})

/* 定时器主测试函数 */
void timer_test();

#endif //TIMER_TEST_H
