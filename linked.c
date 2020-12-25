#include "linked.h"

/* 节点（元数据的）大小 */
const uint NODE_SIZE = sizeof(Linked);

/* 创建节点 */
Linked *linked_create(const uint dataSize) {
    Linked *self = (Linked *) malloc(dataSize + NODE_SIZE);
    lock_init(&self->lock);
    self->next = NULL;
    self->state = Valid;
    return self;
}

/* 空节点，用于返回错误值 */
Linked *linked_new_empty() {
    Linked *self = linked_create(0);
    sinfo("new");
    return self;
}

/* 获取节点的数据指针 */
void *linked_get_data(const Linked *const self) {
    return (void *) self + NODE_SIZE;
}

/* 获取节点锁 */
error linked_lock(Linked *const self) {
    return lock_lock(&self->lock);
}

/* 释放节点锁 */
error linked_unlock(Linked *const self) {
    error err = lock_unlock(&self->lock);
    if (self->state == Invalid) {
        sdebug("invalid");
        err = Error;
    }

    return err;
}

/* 销毁节点 */
error linked_delete(Linked *const self) {
    sinfo("invalidate");
    self->state = Invalid;
    self->next = NULL;
    if (linked_unlock(self) == Error) {
        return Error;
    }

    lock_delete(&self->lock);
    free(self);
    return Success;
}

/* 回收内存 */
error linked_deallocate(Linked *const self, const void *const addr) {
    Linked *last = self;
    sinfo("freeing addr=%p", addr);
    linked_lock(self);
    if (self->next == NULL) {
        sinfo("empty header");
        linked_unlock(self);
        return Error;
    }

    for (; last->next != NULL; last = last->next) {
        linked_lock(last->next);
        if (linked_get_data(last->next) == addr) {
            Linked *const toFree = last->next;
            sinfo("chain next=%p deleted=%p", last->next->next, last->next);
            last->next = last->next->next;
            linked_delete(toFree);
            linked_unlock(last);
            return Success;
        }
        if (linked_unlock(last) == Error) {
            return Error;
        }

        if (last->next->next == NULL) {
            linked_unlock(last->next);
            sinfo("not found");
            return Error;
        }
    }

    sinfo("logic error");
    return Error;
}

/* 追加节点至链表尾 */
Linked *linked_append(Linked *const self, const uint size) {
    Linked *current = self;
    linked_lock(current);
    while (current->next != NULL) {
        linked_lock(current->next);
        linked_unlock(current);
        current = current->next;
    }


    current->next = linked_create(size);
    linked_unlock(current);
    sinfo("next=%p", current->next);
    return current->next;
}

/* 分配内存 */
void *linked_allocate(Linked *const self, const uint size) {
    const Linked *const tail = linked_append(self, size);
    sinfo("tail=%p", tail);
    return linked_get_data(tail);
}

/* 监控动态扩展的内存池 */
void linked_print(Linked *const self, const LOG_LEVEL level) {
    Linked *current = self;
    log_head(level);
    for (; current != NULL; current = current->next) {
        printf("%p->", current);
    }
    log_tail();
}

/* 销毁动态扩展的内存池 */
error linked_clear(Linked *const self) {
    sinfo("clr");
    linked_lock(self);
    while (self->next != NULL) {
        Linked *tail;
        linked_lock(self->next);
        tail = self->next->next;
        sdebug("next=%p tail=%p", self->next, tail);
        linked_delete(self->next);
        self->next = tail;
    }

    linked_delete(self);
    return Success;
}
