#include "xmodem_test.h"

/* 测试函数1 */
xm_test_base(1, {
    tpool_start();
    xmodem_run(&self);
})

/* 测试函数2 */
xm_test_base(2, {
    tpool_start();
    xm_consumer_start(&self.consumer);
    lock_reunlock(&self.consumer.exit);
})

/* 测试函数3 */
xm_test_base(3, {
    tpool_start();
    xm_producer_run(&self.producer);
})

/* 测试函数 */
error xm_test() {
    xm_test_1();
    xm_test_2();
    xm_test_3();
    return Success;
}
