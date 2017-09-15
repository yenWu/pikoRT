#include <kernel/kernel.h>
#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/thread.h>
#include <kernel/slab.h> //malloc

#include "linux/list.h"
#include "bitmap.h"

static LIST_HEAD(mutex_head);

kernel_mutex_t *kernel_mutex_init(void)
{
    struct mutex *kmtx = (struct mutex *) malloc(sizeof(struct mutex)); //should align with bytes

    INIT_BITMAP(kmtx.prio_bmp);
    INIT_LIST_HEAD(kmtx->list);

    struct sync_chain *ch = sync_chain_init();

    return kmtx;
}

void kernel_mutex_destroy(kernel_mutex_t *kmtx)
{
    free(kmtx);
}

int thread_requeue(struct thread_info *thrd)
{
    int state = thrd->state;
    struct bitmap_struct *bmp = NULL;

    switch (state) {
        case THREAD_STATE_ACTIVED:
            bmp = sched_struct->active;
            break;
        case THREAD_STATE_EXPIRED:
            bmp = sched_struct->expire;
        default:
    }
    
    if (bmp) {
        thread_dequeue(thrd, bmp);
        thread_enqueue(thrd, bmp);
    }

    return 0;
}

/* The thread owns the mutex on return. We also check the case when the lock
 * has been released between the test of the mutex and this syscall. */
int sys_pthread_mutex_lock(user_mutex_t *umtx)
{
/*
	mutex->val++;
	if (!mutex->val)
		return 0;
	CURRENT_THREAD_INFO(curr_thread);
	curr_thread->ti_private = mutex;
	curr_thread->ti_state = THREAD_STATE_BLOCKED;
	list_add_tail(&curr_thread->ti_q, &mutex_head);
	sched_elect(SCHED_OPT_NONE);
*/

    kernel_mutex_t *kmtx;

    //BUG_ON(umtx->lock == 0);
    /* create kernel mutex if uncreated */
    if (umtx->create == -1) {//Yet create
        kmtx = kernel_mutex_init();
        kmtx->owner = umtx->thrd << 2;
        kmtx->owner_prio = (umtx->thrd << 2)->priority;
        umtx->kmutex = kmtx >> 2;
    else
        kmtx = umtx->kmutex << 2;
    thread_enqueue(curr_thread, &kmtx->prio_bmp);

    CURRENT_THREAD_INFO(curr_thread);
    struct thread_info *owner = kmtx->owner;
    int state = owner->state;

    /* priority inheritance */
    int max_prio = find_first_bit(&kmtx->prio_bmp);
    if (max_prio > curr_thread->priority) {
        owner->priority = curr_thread->priority;
        thread_requeue(owner);
    }



    /* create thread prio slots for mutex or even semaphore */
    if (!curr_thread->pi_private) {
        curr_thread->pi_private = (void *) malloc(sizeof(struct bitmap_struct));
        INIT_BITMAP((struct bitmap_struct)curr_thread->pi_private);
    }
	bitmap_enqueue(kmtx->list, curr_thread->ti_private)
	curr_thread->ti_state = THREAD_STATE_BLOCKED;

    sched_elect(SCHED_OPT_NONE);

	return 0;
}

static struct thread_info *find_first_blocking_thread(kernel_mutex_t *kmtx)
{
	/*struct thread_info *thread;

	list_for_each_entry(thread, &mutex_head, ti_q) {
		if (thread->ti_private == mutex)
			return thread;
	}*/

    int max_prio = find_first_bit(kmtx->prio_bmp);

	return bitmap_first_entry(kmtx->prio_bmp, max_prio, struct thread_info, ti_q);
}

int sys_pthread_mutex_unlock(user_mutex_t *umtx)
{
/*
	struct thread_info *waiter = NULL;

	mutex->val--;
	if (mutex->val >= 0) {
		waiter = find_first_blocking_thread(mutex);
		if (!waiter) {
			printk("[mutex_unlock] No blocking threads for mutex=<%p>\n",
			       mutex);
			return -1;
		}
		list_del(&waiter->ti_q);
		sched_enqueue(waiter);
	}
	CURRENT_THREAD_INFO(curr_thread);
	if (curr_thread->ti_state == THREAD_STATE_BLOCKED) {
		sched_elect(SCHED_OPT_NONE);
	} else if (waiter && (curr_thread->ti_priority <= waiter->ti_priority)) {
		sched_enqueue(curr_thread);
		sched_elect(SCHED_OPT_NONE);
*/
    mtx

    sched_enqueue();

	return 0;
}
