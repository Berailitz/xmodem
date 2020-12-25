#include "allocator.h"

/* 监控内存管理模块 */
void allocator_print(const Allocator *const self, const LOG_LEVEL level) {
    pool_print(self->pool, level);
    linked_print(self->linked, level);
}

/* 创建模块 */
Allocator *allocator_new(const uint height, const uint width) {
    Allocator *self = malloc(sizeof(Allocator));
    sinfo("new");
    self->pool = pool_new(height, width);
    self->linked = linked_new_empty();
    return self;
}

/* 分配内存 */
void *allocator_allocate(Allocator *const self, const uint size) {
    void *addr;
    sinfo("size=%d", size);
    addr = pool_allocate(self->pool, size);
    if (addr != NULL) {
        sinfo("pool addr=%p", addr);
        return addr;
    }

    addr = linked_allocate(self->linked, size);
    sinfo("linked addr=%p", addr);
    return addr;
}

/* 回收内存 */
error allocator_deallocate(Allocator *const self, const void *const addr) {
    error err = pool_deallocate(self->pool, addr);
    if (err == Success) {
        return Success;
    }

    return linked_deallocate(self->linked, addr);
}

/* 销毁模块 */
error allocator_delete(Allocator *const self) {
    Pool *pool = self->pool;
    Linked *linked = self->linked;
    sinfo("del");
    self->pool = NULL;
    self->linked = NULL;
    pool_clear(pool);
    linked_clear(linked);
    return Success;
}
