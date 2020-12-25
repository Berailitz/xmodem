#include "util.h"

/* 生成随机整数 */
uint rand_uint(const uint m) {
    return rand() % m;
}

/* 等待若干毫秒 */
int msleep(long miliseconds) {
#ifdef __MINGW32__
    struct timespec req;

    if (miliseconds > 999) {
        req.tv_sec = (int) (miliseconds / 1000);
        req.tv_nsec = (miliseconds - ((long) req.tv_sec * 1000)) * 1000000;
    } else {
        req.tv_sec = 0;
        req.tv_nsec = miliseconds * 1000000;
    }

    return nanosleep(&req, NULL);
#else
    taskDelay(sysClkRateGet()*miliseconds/1000);
#endif
}
