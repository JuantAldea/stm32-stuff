#include <stm32f407xx.h>

#include "kernel.h"
#include "config.h"

tcbType tcbs[NUM_OF_THREADS] = { 0 };
tcbType *currentPt;
uint32_t TCB_STACKS[NUM_OF_THREADS][THREAD_STACK_SIZE];

uint32_t MILLIS_PREESCALER;

void osSchedulerLaunch(void);

void osKernelStackInit(int i)
{
    tcbs[i].stackPt = &TCB_STACKS[i][THREAD_STACK_SIZE - 16];

    // set T-bit in PSR -> Operate in thumb mode
    for (int j = 0; j < THREAD_STACK_SIZE; j++) {
        TCB_STACKS[i][j] = 0xDEADC0DE;
    }

    TCB_STACKS[i][THREAD_STACK_SIZE - 1] = xPSR_T_Msk;
    TCB_STACKS[i][THREAD_STACK_SIZE - 2] = 0xFFFFFFFF;
    TCB_STACKS[i][THREAD_STACK_SIZE - 3] = 0x14;
    TCB_STACKS[i][THREAD_STACK_SIZE - 4] = 0x12;
    TCB_STACKS[i][THREAD_STACK_SIZE - 5] = 0x3;
    TCB_STACKS[i][THREAD_STACK_SIZE - 6] = 0x2;
    TCB_STACKS[i][THREAD_STACK_SIZE - 7] = 0x1;
    TCB_STACKS[i][THREAD_STACK_SIZE - 8] = 0x0;

    TCB_STACKS[i][THREAD_STACK_SIZE - 9] = 0x11;
    TCB_STACKS[i][THREAD_STACK_SIZE - 10] = 0x10;
    TCB_STACKS[i][THREAD_STACK_SIZE - 11] = 0x9;
    TCB_STACKS[i][THREAD_STACK_SIZE - 12] = 0x8;
    TCB_STACKS[i][THREAD_STACK_SIZE - 13] = 0x7;
    TCB_STACKS[i][THREAD_STACK_SIZE - 14] = 0x6;
    TCB_STACKS[i][THREAD_STACK_SIZE - 15] = 0x5;
    TCB_STACKS[i][THREAD_STACK_SIZE - 16] = 0x4;
}

uint8_t osKernelAddThreads(void (task0)(void), void (task1)(void), void (task2)(void))
{
    __disable_irq();
    tcbs[0].nextPt = &tcbs[1];
    tcbs[1].nextPt = &tcbs[2];
    tcbs[2].nextPt = &tcbs[0];

    tcbs[0].id = 0;
    tcbs[1].id = 1;
    tcbs[2].id = 2;

    osKernelStackInit(0);
    osKernelStackInit(1);
    osKernelStackInit(2);

    TCB_STACKS[0][THREAD_STACK_SIZE - 2] = (uint32_t) task0;
    TCB_STACKS[1][THREAD_STACK_SIZE - 2] = (uint32_t) task1;
    TCB_STACKS[2][THREAD_STACK_SIZE - 2] = (uint32_t) task2;

    currentPt = &tcbs[0];

    __enable_irq();
    return 0;
}

const uint32_t maxOfPeriodicTasks = 10;
uint32_t numberOfPeriodicTasks = 0;
periodicTask_t PeriodicTasks[10] = { 0 };

uint8_t osAddPeriodicTask(void (task)(void), uint32_t period)
{
    if (maxOfPeriodicTasks == numberOfPeriodicTasks) {
        return 1;
    }

    PeriodicTasks[numberOfPeriodicTasks].period = period;
    PeriodicTasks[numberOfPeriodicTasks].timeLeft = period - 1;
    PeriodicTasks[numberOfPeriodicTasks].task = task;
    numberOfPeriodicTasks++;
    return 0;
}

void osRunPeriodicTasks(void)
{
    for (int i = 0; i < numberOfPeriodicTasks; i++) {
        if (!PeriodicTasks[i].timeLeft--) {
            PeriodicTasks[i].task();
            PeriodicTasks[i].timeLeft = PeriodicTasks[i].period - 1;
        }
    }
}

void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
    osRunPeriodicTasks();
}

void osPeriodicTasks_Init(uint32_t freq, uint8_t priority)
{
    __disable_irq();
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // 16.000.000 / 16 = 1 000 000
    TIM2->PSC = 16 - 1;
    TIM2->ARR = 1000000 / freq - 1;
    TIM2->CNT = 0;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_SetPriority(TIM2_IRQn, priority);
    NVIC_EnableIRQ(TIM2_IRQn);
    __enable_irq();
}

void osKernelInit(void)
{
    MILLIS_PREESCALER = SYS_FREQ / 1000;
}

void osKernelLaunch(uint32_t quanta)
{
    SysTick->CTRL = 0;
    SysTick->VAL = 0;
    SysTick->LOAD = quanta * MILLIS_PREESCALER - 1;
    //low priority
    NVIC_SetPriority(PendSV_IRQn, 0xF);
    NVIC_SetPriority(SysTick_IRQn, 0xF);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    osSchedulerLaunch();
    //__enable_irq();
}

void osSchedulerLaunch(void)
{
    __asm("LDR R0, =currentPt");
    __asm("LDR R2, [R0]");
    __asm("LDR SP, [R2]");

    __asm("POP {R4-R11}");
    __asm("POP {R12}");
    __asm("POP {R0-R3}");
    //skip LR
    __asm("ADD SP, SP,#4");
    __asm("POP {LR}");
    //skip PSR
    __asm("ADD SP, SP,#4");
    __asm("CPSIE I");
    __asm("BX LR");
}

void SysTick_Handler(void)
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    __asm("isb");
}

void osScheduler(void)
{
    currentPt = currentPt->nextPt;
}
/*
 * Upon exception
 *  reverse(R0, R1, R2, R3, R12, LR, PC, PSR)
 * are pushed onto the stack
 */
__attribute__((naked)) void PendSV_Handler(void)
{
    // disable global interrupts
    __asm("CPSID I");
    // save registers R[4-11]
    __asm("PUSH {R4-R11}");
    //pointer currentPt points to tbc.stackPtr, store currentPt in R0
    __asm("LDR R0, =currentPt");

    //dereference R0 to get pointer tbc.stackPtr
    __asm("LDR R1, [R0]");
    //store SP in *tbc.stackPtr (or tbc->stackPtr)
    __asm("STR SP, [R1]");

    __asm("PUSH {R0, LR}");
    __asm("BL osScheduler");
    __asm("POP {R0, LR}");

    //choose next thread
    //R1 points to tbc (or tbc.stackPtr). tbc.nextPt is located 4 bytes (32bits) above.
    //__asm("LDR R1, [R1, #4]");
    // R0 still points to currentPt
    //__asm("STR R1, [R0]");

    //R1 = *currentPtr
    __asm("LDR R1, [R0]");
    //load SP from currentPt->stackPt
    __asm("LDR SP, [R1]");
    // restore registers
    __asm("POP {R4-R11}");
    //enable global interrupts
    __asm("CPSIE I");
    // return from exception and restore R0-R3, R12, lr, pc, psr
    __asm("BX LR");
}

void osThreadYield(void)
{
    //reset time
    SysTick->VAL = 0;
    //trigger SysTick
    SCB->ICSR |= SCB_ICSR_PENDSTSET_Msk;
}
/*
 void timer2_1hz_init(void)
 {
 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
 // 16.000.000 / 1600 = 10 000
 TIM2->PSC = 1600 - 1;
 // 10000/10000 = 1
 TIM2->ARR = 10000 - 1;
 TIM2->CNT = 0;
 TIM2->DIER |= TIM_DIER_UIE;
 TIM2->CR1 |= TIM_CR1_CEN;
 NVIC_EnableIRQ(TIM2_IRQn);
 }
 */
void osSemaphoreInit(volatile int32_t *semaphore, int32_t value)
{
    *semaphore = value;
}

void osSemaphoreGive(volatile int32_t *semaphore)
{
    __disable_irq();
    ++(*semaphore);
    __enable_irq();
}

void osSemaphoreTakeSpin(volatile int32_t *semaphore)
{
    __disable_irq();

    while ((*semaphore) <= 0) {
        __enable_irq();
        __disable_irq();
    }

    --(*semaphore);
    __enable_irq();
}

void osSemaphoreTakeYield(volatile int32_t *semaphore)
{
    __disable_irq();

    while ((*semaphore) <= 0) {
        __enable_irq();
        osThreadYield();
        __disable_irq();
    }

    --(*semaphore);
    __enable_irq();
}

#define FIFO_MAX_LEN 10
struct fifo_t {
    volatile uint32_t putIdx;
    volatile uint32_t getIdx;
    volatile int32_t data[FIFO_MAX_LEN];
    volatile uint32_t current_size;
    volatile int32_t semaphore;
    volatile int32_t lost_data;
};

void osFifoInit(struct fifo_t *fifo)
{
    *fifo = (struct fifo_t ) { 0 };
}

uint32_t osFifoPut(struct fifo_t *fifo, int32_t data)
{
    osSemaphoreTakeYield(&(fifo->semaphore));
    if (fifo->current_size == FIFO_MAX_LEN) {
        osSemaphoreGive(&(fifo->semaphore));
        return 1;
    }

    fifo->data[fifo->putIdx] = data;
    fifo->putIdx = (fifo->putIdx + 1) % FIFO_MAX_LEN;
    fifo->current_size++;

    osSemaphoreGive(&(fifo->semaphore));
    return 0;
}

int32_t osFifoGet(struct fifo_t *fifo)
{
    osSemaphoreTakeYield(&fifo->semaphore);

    while(!fifo->current_size){
        osSemaphoreGive(&(fifo->semaphore));
        osSemaphoreTakeYield(&(fifo->semaphore));
    }

    int32_t data = fifo->data[fifo->getIdx];
    fifo->getIdx = (fifo->getIdx + 1) % FIFO_MAX_LEN;
    fifo->current_size--;
    osSemaphoreGive(&(fifo->semaphore));
    return data;
}
