#include "xmodem_test.h"

error xm_test() {
    xmodem xm;

    tpool_init();
    xmodem_init(&xm);

    tpool_start();
    xmodem_run(&xm);

    tpool_stop();

    return Success;
}
