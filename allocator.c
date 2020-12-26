#include "allocator.h"

/* 默认大小 */
const uint ALLOCATOR_DEFAULT_HEIGHT = 8;
const uint ALLOCATOR_DEFAULT_WIDTH = 16;

/* 监控内存管理模块 */
void allocator_print(const Allocator *const self, const LOG_LEVEL level) {
    pool_print(self->pool, level);
    linked_print(self->linked, level);
}

/* 创建模块 */
Allocator *allocator_new(uint height, uint width) {
    if (height == 0) {
        height = ALLOCATOR_DEFAULT_HEIGHT;
    }

    if (width == 0) {
        width = ALLOCATOR_DEFAULT_WIDTH;
    }

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

    addr = malloc(size);
    sinfo("linked addr=%p", addr);
    return addr;
}

/* 回收内存 */
error allocator_deallocate(Allocator *const self, void *const addr) {
    error err = pool_deallocate(self->pool, addr);
    if (err == Success) {
        return Success;
    }

    free(addr);
    return Success;
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
