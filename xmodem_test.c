#include "xmodem_test.h"

xm_test_base(1, {
    tpool_start();
    xmodem_run(&xm);
})

error xm_test() {
    xm_test_1();
    return Success;
}
