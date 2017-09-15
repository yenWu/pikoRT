#include <kernel/syscalls.h>
#include <kernel/linkage.h>

	.syntax unified
	.thumb

/**
 *  <pthread.h>: typedef pthread_mutex_t uint32_t
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
 *      A: 0xfffffff? = unlocked
 *         if BC == 00, A = pointer of kernel_mutex_t
 *         if BC == 10, A = pointer of struct thread_info
 */
	// -1: unlocked, 0: locked, positive: locked, possible waiters

	@ int pthread_mutex_lock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_lock)
	@movs	r2, #0
	mov 	r3, r0
0:	ldrex	r1, [r3]
    tst r1, #1			@ check locked?
	beq	1f              @ if locked, jmp to 1
	bx	current_thread_info
    orr r0, #2          @not yet create mutex obj
	strex	r1, r0, [r3]
    teq r1, #0			@ 'strex' success?
	beq	0b
	dmb			@ ARMv7-M ARM, A3.4.6
	mov	r0, #0		@ it also update EQ flag
    bx  lr
    /**
     * already locked, should call sys_pthread_mutex_lock()
     */
1:	mov	r1, #SYS_PTHREAD_MUTEX_LOCK
	svc	#1
	bx	lr
ENDPROC(pthread_mutex_lock)

	@ int pthread_mutex_trylock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_trylock)
	@movs	r2, #0
	movs	r3, #0
0:	ldrex	r1, [r3]
    tst r1, #1			@ check locked?
    beq 1f              @ if locked, jmp to 1
	bx	current_thread_info
    orr r0, #2          @not yet create mutex obj
	strex	r1, r0, [r3]
	teq	r1, #0			@ 'strex' success?
    bne 1f
	mov	r0, #0		@ it also update EQ flag
    bx  lr
    /**
     * already locked, return -1 without sleep
     */
1:  mov r0, #-1
	bx	lr
ENDPROC(pthread_mutex_trylock)

	@ int pthread_mutex_unlock(pthread_mutex_t *mutex)
ENTRY(pthread_mutex_unlock)
	movs	r2, #-1
0:	ldrex	r1, [r0]
	tst	r1, #3			@ Just one hold lock?
    beq 1f              @ if unlocked or mutiple waiters, jmp to 1
	strex	r1, r2, [r0]
	teq	r1, #0			@ 'strex' success?
	bne	0b
	dmb			@ ARMv7-M ARM, A3.4.6
	mov	r0, #0		@ it also update EQ flag
    bx lr
    /**
     * mutiple waiters, should call sys_pthread_mutex_unlock()
     *
     * FIXME: We don't check unlocked or not.
     */
1:	mov	r1, #SYS_PTHREAD_MUTEX_UNLOCK
	svc	#1
	bx	lr
ENDPROC(pthread_mutex_unlock)
