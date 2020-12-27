#ifndef XMODEM_TEST_H
#define XMODEM_TEST_H

#include "xmodem.h"

/* 测试函数框架 */
#define xm_test_base(name, test_block) \
error xm_test_##name() {               \
    xmodem self; \
 \
    plog_sep(level, "TEST "#name" START"); \
 \
    xmodem_init(&self); \
 \
    tpool_start(); \
 \
    test_block \
 \
    tpool_stop(); \
 \
    plog_sep(level, "TEST "#name" END"); \
 \
    return Success; \
}

/* 测试函数 */
error xm_test();

#endif //XMODEM_TEST_H
