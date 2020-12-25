#include "timer_test.h"

/* 测试1：单一定时器 */
ttest(1, {
    tpool_register(ttest_callback_1, (void *) 0, 1, 0);
})

/* 测试2：多个定时器 */
ttest(2, {
    tpool_register(ttest_callback_2, (void *) 0, 2, 0);
    tpool_register(ttest_callback_2, (void *) 1, 1, 0);
})

/* 测试3：周期性定时器 */
ttest(3, {
    tpool_register(ttest_callback_3, (void *) 0, 3, 0);
    tpool_register(ttest_callback_3, (void *) 1, 1, 1);
})

/* 测试4：带周期的定时器和单次定时器 */
ttest(4, {
    tpool_register(ttest_callback_4, (void *) 0, 5, 0);
    tpool_register(ttest_callback_4, (void *) 1, 1, 1);
    tpool_register(ttest_callback_4, (void *) 1, 2, 2);
})

/* 测试5：更复杂的测试 */
ttest(5, {
    tpool_register(ttest_callback_5, (void *) 0, 30, 0);
    tpool_register(ttest_callback_5, (void *) 1, 1, 1);
    tpool_register(ttest_callback_5, (void *) 1, 2, 2);
    tpool_register(ttest_callback_5, (void *) 1, 3, 0);
    tpool_register(ttest_callback_5, (void *) 1, 9, 0);
})

/* 定时器主测试函数 */
void timer_test() {
    ttest_5();
}
