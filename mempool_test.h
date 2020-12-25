#ifndef MEMPOOL_TEST_H
#define MEMPOOL_TEST_H

#include "linked.h"
#include "pool.h"
#include "allocator.h"
#include "lock.h"
#include "util.h"
#include "concurrent.h"
#include "test.h"
#include <unistd.h>

/* 分配内存并监控模块运行状态 */
#define saprint(ip, size) tester_allocate(self, 0, pss[0], ip, size)
/* 回收内存并监控模块运行状态 */
#define sdprint(ip) tester_deallocate(self, 0, pss[0], ip)

/* 最多可以有多少个模块（内存池） */
#define MAX_ALLOCATOR_NUM 5
typedef struct {
    Allocator *allocators[MAX_ALLOCATOR_NUM];
} MPTester;

/* 初始化测试结构 */
error tester_init(MPTester *const self);

/* 销毁测试结构 */
error tester_clear(MPTester *const self);

/* 分配测试结构 */
void tester_allocate(MPTester *self, const uint ia, void **const ps, const uint ip, const uint size);

/* 回收测试结构 */
void tester_deallocate(MPTester *self, const uint ia, void **const ps, const uint ip);

/* 分配第一个测试结构 */
#define tester_new_first() self->allocators[0] = allocator_new(height, width)
/* 回收第一个测试结构 */
#define tester_delete_first() allocator_delete(self->allocators[0])

/* 最多同时持有多少个已分配的空间 */
const uint MAX_POINTERS_NUM;

/* 初始化指针数组 */
void **tester_init_ps();

/* 定义一个测试函数 */
#define atest(name, block) error test_allocator_##name(const uint height, const uint width) { \
    MPTester tester;                              \
    MPTester *self = &tester;                                              \
    error err = Success;                                               \
    void **pss[MAX_POINTERS_NUM];                                         \
    uint i = 0;                                                                     \
    slog_sep(level, "TEST "#name" START"); \
    tester_init(self);                                                  \
    for (; i < MAX_ALLOCATOR_NUM; i++) {                                                      \
        pss[i] = tester_init_ps();                                          \
    }                                                                 \
                       \
    block                                                               \
                                                                                              \
    tester_clear(self);                           \
    for (; i < MAX_ALLOCATOR_NUM; i++) {                                                      \
        free(pss[i]);                                          \
    }                                                                                           \
    slog(level, "TEST "#name" err=%d\n", err);                              \
    return err;                                                                 \
}

/*分配与回收的次数 */
const uint TEST_7_NUM;
/*分配与回收的最大大小 */
const uint TEST_7_MAX_SIZE;

/* 内存池的大小 */
const uint TEST_8_SHAPES[5][2];

/* 并行执行的任务数量 */
#define CONCURRENT_TASK_NUM 2

/* 存储并行执行的任务 */
MPTester *testers[CONCURRENT_TASK_NUM];

/* 存储并行执行的任务序号 */
int TesterIs[CONCURRENT_TASK_NUM];

#define tester_init_5() int it; \
    for (it = 0; it < MAX_ALLOCATOR_NUM; it++) { \
        self->allocators[it] = allocator_new(TEST_8_SHAPES[it][0], TEST_8_SHAPES[it][1]);                                 \
    }                                \
    for (it = 0; it < CONCURRENT_TASK_NUM; it++) { \
        testers[it] = self;      \
        TesterIs[it] = it;\
    }

/* 被并行执行的任务 */
void *tester_cross(void *ii);

/* 调用测试程序 */
#define ta(name) test_allocator_##name(height, width)

/* 测试分配模块 */
int test_allocator();

#endif
