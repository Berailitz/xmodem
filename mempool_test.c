#include "mempool_test.h"

/* 初始化测试结构 */
error tester_init(MPTester *const self) {
    memset(self->allocators, 0, MAX_ALLOCATOR_NUM * sizeof(Allocator *));
    return Success;
}

/* 销毁测试结构 */
error tester_clear(MPTester *const self) {
    uint i = 0;
    for (; i < MAX_ALLOCATOR_NUM; i++) {
        if (self->allocators[i] != NULL) {
            allocator_delete(self->allocators[i]);
            self->allocators[i] = NULL;
        }
    }

    return Success;
}

/* 分配测试结构 */
void tester_allocate(MPTester *self, const uint ia, void **const ps, const uint ip, const uint size) {
    sdebug("%d NEW %d SIZE %d", ia, ip, size);
    ps[ip] = allocator_allocate(self->allocators[ia], size);
    allocator_print(self->allocators[ia], level);
}

/* 回收测试结构 */
void tester_deallocate(MPTester *self, const uint ia, void **const ps, const uint ip) {
    sdebug("%d DEL %d ADDR %p", ia, ip, ps + ip);
    allocator_deallocate(self->allocators[ia], ps[ip]);
    allocator_print(self->allocators[ia], level);
}

/* 最多同时持有多少个已分配的空间 */
const uint MAX_POINTERS_NUM = 50;

/* 初始化指针数组 */
void **tester_init_ps() {
    const uint psSize = MAX_POINTERS_NUM * sizeof(void *);
    void **ps = malloc(psSize);
    memset(ps, (int) 0, psSize);
    return ps;
}

/*测试程序1：分配与回收1个小内存块*/
atest(1, {
    tester_new_first();

    saprint(0, 8);
    sdprint(0);
})

/*测试程序2：分配与回收多个小内存块*/
atest(2, {
    tester_new_first();

    saprint(0, 8);
    saprint(1, 16);
    sdprint(0);
    sdprint(1);
})

/*测试程序3：分配与回收多个小内存块*/
atest(3, {
    tester_new_first();

    saprint(0, 8);
    saprint(1, 13);
    sdprint(1);
    sdprint(0);
})

/*测试程序4：分配与回收更多个或大或小的内存块 */
atest(4, {
    tester_new_first();

    saprint(0, 8);
    saprint(1, 22);
    saprint(2, 15);
    sdprint(1);
    sdprint(2);
    sdprint(0);
})

/*测试程5：分配与回收更多个或大或小的内存块 */
atest(5, {
    tester_new_first();

    saprint(0, 22);
    saprint(1, 32);
    sdprint(1);
    sdprint(0);
})

/*测试程6：分配与回收更多个或大或小的内存块 */
atest(6, {
    tester_new_first();

    saprint(0, 22);
    saprint(1, 11);

    saprint(2, 10);
    saprint(3, 21);
    saprint(4, 32);

    sdprint(3);
    sdprint(2);

    saprint(5, 43);
    saprint(6, 64);

    sdprint(6);
    sdprint(5);
    sdprint(4);

    sdprint(0);
    sdprint(1);
})

/*分配与回收的次数 */
const uint TEST_7_NUM = 40;
/*分配与回收的最大大小 */
const uint TEST_7_MAX_SIZE = 70;

/*测试7：分配与回收更多个或大或小的内存块 */
atest(7, {
    uint test_i = 0;
    tester_new_first();

    for (; test_i < TEST_7_NUM; test_i++) {
        const uint ip = rand_uint(MAX_POINTERS_NUM);
        if (pss[0][ip] == NULL) {
            const uint size = rand_uint(TEST_7_MAX_SIZE);
            saprint(ip, 1);
        } else {
            sdprint(ip);
        }
    }
})

/* 内存池的大小 */
const uint TEST_8_SHAPES[5][2] = {
        {8,   6},
        {8,   10},
        {64,  6},
        {96,  10},
        {128, 3},
};

/* 被并行执行的任务 */
void *tester_cross(void *ii) {
    MPTester *self;
    void **ps = tester_init_ps();
    uint test_i;
    self = testers[*(const int *const)ii];
    for (test_i = 0; test_i < TEST_7_NUM; test_i++) {
        const uint ia = rand_uint(MAX_ALLOCATOR_NUM);
        const uint ip = rand_uint(MAX_POINTERS_NUM);
        if (ps[ip] == NULL) {
            const uint size = rand_uint(TEST_7_MAX_SIZE);
            tester_allocate(self, ia, ps, ip, size);
        } else {
            tester_deallocate(self, ia, ps, ip);
        }
        msleep(1000);
    }
    free(ps);
    return NULL;
}

/* 测试程序8：穿行执行的任务 */
atest(8, {
    tester_init_5();

    tester_cross(0);
})

/* 测试程序9：更多并行执行的任务 */
atest(9, {
    int ie = 0;
    Executor exes[CONCURRENT_TASK_NUM];
    tester_init_5();

    for (ie = 0; ie < CONCURRENT_TASK_NUM; ie++) {
        executor_start(&exes[ie], tester_cross, &TesterIs[ie]);
    }

    msleep(2000);
    /*for (ie = 0; ie < CONCURRENT_TASK_NUM; ie++) {
        executor_join(&exes[ie]);
    }*/

    tester_clear(self);
})

/* 测试分配模块 */
int test_allocator() {
    const uint height = 8, width = 8;
    DE_SELF
    slog_sep(level, "TEST ALLOCATOR START");

    ta(9);

    slog_sep(level, "TEST ALLOCATOR END");

    return 0;
}
