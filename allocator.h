#ifndef MEMPOOL_ALLOCATOR_H
#define MEMPOOL_ALLOCATOR_H

#include "pool.h"
#include "linked.h"

/* 内存管理模块的结构，包括基本内存池和动态扩展内存池 */
typedef struct {
    Pool *pool;
    Linked *linked;
} Allocator;

/* 监控内存管理模块 */
void allocator_print(const Allocator *const self, const LOG_LEVEL level);

/* 创建模块 */
Allocator *allocator_new(const uint height, const uint width);

/* 分配内存 */
void *allocator_allocate(Allocator *const self, const uint size);

/* 回收内存 */
error allocator_deallocate(Allocator *const self, const void *const addr);

/* 销毁模块 */
error allocator_delete(Allocator *const self);

#endif //MEMPOOL_ALLOCATOR_H
