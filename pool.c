#include "pool.h"

/* 空块，作为异常返回值 */
Block EmptyBlock;

/* 非法块的id，作为异常返回值 */
const uint INVALID_BLOCK_ID;

/* 块头的大小 */
const uint BLOCK_HEADER_SIZE;

/* 获取块的数据地址 */
Block *pool_get_data(const Pool *self) {
    return (void *) self + POOL_META_SIZE;
}

/* 获取块的大小 */
uint pool_block_size(const Pool *self) {
    return self->meta.height + BLOCK_HEADER_SIZE;
}

/* 初始化块 */
error block_init(Block *const self) {
    return lock_init(&self->header.lock);
}

/* 锁定块 */
error block_lock(Block *const self) {
    return lock_lock(&self->header.lock);
}

/* 解锁块 */
error block_unlock(Block *const self) {
    return lock_unlock(&self->header.lock);
}

/* 块id换块地址 */
Block *pool_block_id_to_block(const Pool *const self, const uint blockID) {
    if (blockID >= self->meta.width) {
        serror("overflow blockID=%d", blockID);
        return NULL;
    }

    return (Block *) ((void *) pool_get_data(self) + pool_block_size(self) * blockID);
}

/* 创建基本内存池 */
Pool *pool_new(const uint height, const uint width) {
    const uint blockSize = height + BLOCK_HEADER_SIZE;
    const uint poolSize = POOL_META_SIZE + blockSize * width;
    uint i = 0;
    Pool *self = (Pool *) malloc(poolSize);
    uint size;

    self->meta.height = height;
    self->meta.width = width;

    size = pool_block_size(self) * width;
    memset(pool_get_data(self), 0, size);

    for (; i < width; i++) {
        Block *const block = pool_block_id_to_block(self, i);
        block_init(block);
    }

    sinfo("height=%d width=%d start=%p end=%p", self->meta.height, self->meta.width,
          ((void *) self) + POOL_META_SIZE, ((void *) self) + poolSize);
    return self;
}

/* 监控基本内存池 */
void pool_print(const Pool *const self, const LOG_LEVEL level) {
    uint sizeLeft = 0;
    uint i = 0;

    log_head(level);
    for (; i < self->meta.width; ++i) {
        Block *const block = pool_block_id_to_block(self, i);
        if (sizeLeft > 0) {
            if (block->header.size > 0) {
                printf("?");
            } else {
                printf("*");
            }

            sizeLeft--;
        } else {
            if (block->header.size > 0) {
                sizeLeft = block->header.size - 1;
                printf("^");
            } else {
                printf("O");
            }
        }
    }

    log_tail();
}

/* 是否是空块 */
bool block_is_empty(const Block *self) {
    return self == &EmptyBlock;
}

/* 地址换块id */
uint pool_pointer_to_block_id(const Pool *const self, const void *const p) {
    const Block *const block = p - BLOCK_HEADER_SIZE;
    const Block *const data = pool_get_data(self);
    if (block < data) {
        sinfo("before data");
        return INVALID_BLOCK_ID;
    } else {
        const uint offset = (void*)block - (void*)data;
        const uint blockSize = self->meta.height + BLOCK_HEADER_SIZE;
        const uint blockID = offset / blockSize;
        if (offset % blockSize != 0) {
            sinfo("not start of block p=%p", block);
            return INVALID_BLOCK_ID;
        }

        if (blockID >= self->meta.width) {
            sinfo("after end");
            return INVALID_BLOCK_ID;
        }

        sinfo("blockID=%d", blockID);
        return blockID;
    }
}

/* 数据地址换 块地址*/
Block *pool_pointer_to_block(const Pool *const self, const void *const addr) {
    const uint blockID = pool_pointer_to_block_id(self, addr);
    if (blockID == INVALID_BLOCK_ID) {
        serror("invalid");
        return &EmptyBlock;
    } else {
        Block *const block = (Block *) (addr - BLOCK_HEADER_SIZE);
        sinfo("block=%p", block);
        return block;
    }
}

/* 解锁多个块 */
error pool_unlock_blocks(const Pool *const self, const uint start, const uint length) {
    uint i = 0;
    for (; i < length; i++) {
        Block *block = pool_block_id_to_block(self, start + i);
        block_unlock(block);
    }

    return Success;
}

/* 查找可用的块 */
uint pool_find_available_block(const Pool *const self, const uint numBlocks) {
    int blocksFound = 0;
    uint i = 0;
    uint usedLeft = 0;
    sdebug("numBlocks=%d", numBlocks);
    for (; i < self->meta.width; ++i) {
        if (usedLeft > 0) {
            usedLeft--;
            sdebug("left=%d", usedLeft);
        } else {
            Block *const block = pool_block_id_to_block(self, i);

            block_lock(block);
            if (block->header.size == 0) {
                blocksFound++;
                sdebug("available block=%p blocksFound=%d", block, blocksFound);

                if (blocksFound >= numBlocks) {
                    uint start = i - numBlocks + 1;
                    sdebug("start=%d blockID=%d", start, block);
                    return start;
                }
            } else {
                usedLeft = block->header.size - 1;
                sdebug("unavailable block=%p", block);
                pool_unlock_blocks(self, i - blocksFound, blocksFound + 1);
                blocksFound = 0;
            }
        }
    }

    pool_unlock_blocks(self, i - blocksFound, blocksFound);
    sinfo("not match");
    return INVALID_BLOCK_ID;
}

/* 标记为不可用 */
error block_mark_unavailable(Block *const self, const uint numBlocks) {
    if (self->header.size > 0) {
        serror("already used");
        return Error;
    }

    self->header.size = numBlocks;
    sdebug("ok");
    return Success;
}

/* 获取块的数据地址 */
void *block_get_data(const Block *const block) {
    return ((void *) block) + BLOCK_HEADER_SIZE;
}

/* 分配内存 */
void *pool_allocate(Pool *const self, const uint size) {
    const uint numBlocks = ceil((double) size / self->meta.height);
    uint blockID = pool_find_available_block(self, numBlocks);
    sdebug("size=%d numBlocks=%d", size, numBlocks);
    if (blockID == INVALID_BLOCK_ID) {
        serror("invalid");
        return NULL;
    } else {
        Block *const block = pool_block_id_to_block(self, blockID);
        sinfo("blockID=%d block=%p", blockID, block);
        block_mark_unavailable(block, numBlocks);
        pool_unlock_blocks(self, blockID, numBlocks);
        return block_get_data(block);
    }
}

/* 查询块的id */
uint pool_block_to_block_id(const Pool *const self, const Block *const block) {
    return pool_pointer_to_block_id(self, block_get_data(block));
}

/* 标记为可用 */
error block_mark_available(Block *const self) {
    if (self->header.size == 0) {
        serror("already available");
        return Error;
    }

    self->header.size = 0;
    sdebug("ok");
    return Success;
}

/* 回收内存 */
error pool_deallocate(Pool *const self, const void *const addr) {
    Block *block;
    sinfo("addr=%p", addr);
    block = pool_pointer_to_block(self, addr);
    sinfo("size=%d", block->header.size);
    if (block_is_empty(block)) {
        sinfo("empty");
        return Error;
    }

    return block_mark_available(block);
}

/* 销毁块 */
error block_delete(Block *const self) {
    block_lock(self);
    if (self->header.size > 0) {
        block_unlock(self);
        return Error;
    }

    lock_delete(&self->header.lock);
    return Success;
}

/* 销毁基本内存池 */
error pool_clear(Pool *const self) {
    uint i = 0;
    sinfo("clr");
    for (; i < self->meta.width; i++) {
        Block *const block = pool_block_id_to_block(self, i);
        block_delete(block);
    }
    free(self);
    return Success;
}
