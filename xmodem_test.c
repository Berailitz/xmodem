#include "xmodem_test.h"

error xm_test() {
    xmodem xm;

    tpool_init();
    xmodem_init(&xm);

    tpool_start();
    xmodem_run(&xm);

    return Success;
}
