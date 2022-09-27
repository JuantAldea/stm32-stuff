#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>

#define NUM_OF_THREADS 3
#define THREAD_STACK_SIZE 100

typedef struct tcb {
    uint32_t *stackPt;
    struct tcb *nextPt;
    uint32_t id;
} tcbType;

typedef struct periodic {
    uint32_t period;
    uint32_t timeLeft;
    void (*task)(void);
}periodicTask_t;

void osKernelInit(void);
void osKernelLaunch(uint32_t quanta);
uint8_t osKernelAddThreads(void (task0)(void), void (task1)(void), void (task2)(void));
void osThreadYield(void);
void timer2_1hz_init(void);

void osSemaphoreInit(volatile int32_t *semaphore, int32_t value);
void osSemaphoreTakeYield(volatile int32_t *semaphore);
void osSemaphoreTakeSpin(volatile int32_t *semaphore);
void osSemaphoreGive(volatile int32_t *semaphore);


void osPeriodicTasks_Init(uint32_t freq, uint8_t priority);
uint8_t osAddPeriodicTask(void (task)(void), uint32_t period);

#endif /* KERNEL_H_ */
