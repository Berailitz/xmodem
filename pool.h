#ifndef MEMPOOL_POOL_H
#define MEMPOOL_POOL_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "statusdef.h"
#include "lock.h"
#include "log.h"

/* 基本内存池的元数据 */
typedef struct PoolMeta {
    uint height, width;
} PoolMeta;

/* 元数据的大小 */
const uint POOL_META_SIZE;

/* 基本内存池 */
typedef struct {
    PoolMeta meta;
} Pool;

/* 块的元数据 */
typedef struct {
    uint size;
    Lock lock;
} BlockHeader;

/* 块 */
typedef struct {
    BlockHeader header;
} Block;

/* 空块，作为异常返回值 */
Block EmptyBlock;

/* 非法块的id，作为异常返回值 */
const uint INVALID_BLOCK_ID;

/* 块头的大小 */
const uint BLOCK_HEADER_SIZE ;

/* 获取块的数据地址 */
Block *pool_get_data(const Pool *self);

/* 获取块的大小 */
uint pool_block_size(const Pool *self);

/* 初始化块 */
error block_init(Block *const self);

/* 锁定块 */
error block_lock(Block *const self);

/* 解锁块 */
error block_unlock(Block *const self);

/* 块id换块地址 */
Block *pool_block_id_to_block(const Pool *const self, const uint blockID);

/* 创建基本内存池 */
Pool *pool_new(const uint height, const uint width);

/* 监控基本内存池 */
void pool_print(const Pool *const self, const LOG_LEVEL level);

/* 是否是空块 */
bool block_is_empty(const Block *self);

/* 地址换块id */
uint pool_pointer_to_block_id(const Pool *const self, const void *const p);

/* 数据地址换 块地址*/
Block *pool_pointer_to_block(const Pool *const self, const void *const addr);

/* 解锁多个块 */
error pool_unlock_blocks(const Pool *const self, const uint start, const uint length);

/* 查找可用的块 */
uint pool_find_available_block(const Pool *const self, const uint numBlocks);

/* 标记为不可用 */
error block_mark_unavailable(Block *const self, const uint numBlocks);

/* 获取块的数据地址 */
void *block_get_data(const Block *const block);

/* 分配内存 */
void *pool_allocate(Pool *const self, const uint size);

/* 查询块的id */
uint pool_block_to_block_id(const Pool *const self, const Block *const block);

/* 标记为可用 */
error block_mark_available(Block *const self);

/* 回收内存 */
error pool_deallocate(Pool *const self, const void *const addr);

/* 销毁块 */
error block_delete(Block *const self);

/* 销毁基本内存池 */
error pool_clear(Pool *const self);

#endif //MEMPOOL_POOL_H
