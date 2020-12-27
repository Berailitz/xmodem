#include "xmodem_test.h"

xm_test_base(1, {
    tpool_start();
    xmodem_run(&self);
})

xm_test_base(2, {
    tpool_start();
    xm_consumer_start(&self.consumer);
    lock_reunlock(&self.consumer.exit);
})

xm_test_base(3, {
    tpool_start();
    xm_producer_run(&self.producer);
})

error xm_test() {
    xm_test_1();
    xm_test_2();
    xm_test_3();
    return Success;
}
