#ifndef MEMPOOL_LINKED_H
#define MEMPOOL_LINKED_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "statusdef.h"
#include "log.h"
#include "lock.h"

/* 节点 */
typedef struct Linked {
    State state;
    Lock lock;
    struct Linked *next;
} Linked;

/* 节点（元数据的）大小 */
const uint NODE_SIZE;

/* 创建节点 */
Linked *linked_create(const uint dataSize);

/* 空节点，用于返回错误值 */
Linked *linked_new_empty();

/* 获取节点的数据指针 */
void *linked_get_data(const Linked *const self);

/* 获取节点锁 */
error linked_lock(Linked *const self);
/* 释放节点锁 */
error linked_unlock(Linked *const self);

/* 销毁节点 */
error linked_delete(Linked *const self);

/* 回收内存 */
error linked_deallocate(Linked *const self, const void *const addr);

/* 追加节点至链表尾 */
Linked *linked_append(Linked *const self, const uint size);

/* 分配内存 */
void *linked_allocate(Linked *const self, const uint size);

/* 监控动态扩展的内存池 */
void linked_print(Linked *const self, const LOG_LEVEL level);

/* 销毁动态扩展的内存池 */
error linked_clear(Linked *const self);

#endif //MEMPOOL_LINKED_H
