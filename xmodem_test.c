#include "xmodem_test.h"

xm_test_base(1, {
    tpool_start();
    xmodem_run(&self);
})

xm_test_base(2, {
    tpool_start();
    xm_consumer_start(&self.consumer);
})

error xm_test() {
    xm_test_1();
    return Success;
}
