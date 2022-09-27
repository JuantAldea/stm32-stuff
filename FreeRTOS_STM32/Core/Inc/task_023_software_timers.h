#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"

static TimerHandle_t xAutoReloadTimer, xOneShotTimer;
static BaseType_t xTimer1Started, xTimer2Started;

void prvOneShotTimerCallback(TimerHandle_t xTimer);
void prvAutoReloadTimerCallback(TimerHandle_t xTimer);

void task_file_main(void)
{
    printf("init finished\r\n");

    xOneShotTimer = xTimerCreate("OneShot", pdMS_TO_TICKS(1000), pdFALSE, NULL, prvOneShotTimerCallback);
    xAutoReloadTimer = xTimerCreate("AutoReload", pdMS_TO_TICKS(5000), pdTRUE, NULL, prvAutoReloadTimerCallback);

    if (!xOneShotTimer || !xAutoReloadTimer) {
        while (1) {}
    }

    xTimer1Started = xTimerStart(xOneShotTimer, 1000);
    xTimer2Started = xTimerStart(xAutoReloadTimer, 1000);

    if (xTimer1Started != pdPASS || xTimer2Started != pdPASS) {
        while (1) {}
    }

}

void prvOneShotTimerCallback(TimerHandle_t xTimer)
{
    static TickType_t xTimeNow;
    xTimeNow = xTaskGetTickCount();
    printf("%s %ld\n\r", __PRETTY_FUNCTION__, xTimeNow);
}

void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
    static TickType_t xTimeNow;
    static int trigger_count = 0;
    xTimeNow = xTaskGetTickCount();
    printf("%s: %ld\n\r", __PRETTY_FUNCTION__, xTimeNow);
    if (++trigger_count == 3){
        printf("Stopping\n\r");
        xTimerStop(xTimer, 0);
    }

}

#endif /* INC_TASK_000_H_ */
