#ifndef _KERNEL_MUTEX_H
#define _KERNEL_MUTEX_H

#include "kernel/bitmap.h"
typedef _Atomic int atomic_s32;

/**
 *  pthread.h: typedef pthread_mutex_t uint32_t
 *
 *    ----------------------
 *    |    30      | 1 | 1 |
 *    ----------------------
 *          A        B   C
 *
 *  Initial with `-1`
 *
 *  Bit format defined:
 *
 *      C: 0 = locked, 1 = unlocked
 *      B: 0 = already create mutex obj in kernel
 *         1 = contrary
 *      A: 0xffffffff = unlocked
 *         otherwise  = pointer of kernel_mutex_t
 */

typedef struct{
    struct thread_info      *owner;
    //unsigned long         owner_prio;
    struct list_head        list;
    struct bitmap_struct    prio_bmp;
    //kernel_mutex_t        *next;
} kernel_mutex_t;

// 0 be true, -1 be false
typedef struct {
    atomic_s32              lock: 1;
    atomic_s32              create: 1;
    union {
        kernel_mutex_t      *kmutex: 30;
        struct thread_info  *thrd: 30;
    };
} user_mutex_t;

int sys_pthread_mutex_lock(kernel_mutex_t *mutex);
int sys_pthread_mutex_unlock(kernel_mutex_t *mutex);

#endif /* !_KERNEL_MUTEX_H */
