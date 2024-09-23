#include "task/timer.h"

static const char *TAG = "timer";
static timer_count = 0;

void vTimerCallback(void *arg) {
    timer_count++;
}

void task_timer(void *arg){
    second_timer = xTimerCreate("timer",
                                pdms_TO_TICKS(1000),
                                pdTrue,
                                (void *) 0,
                                vTimerCallback);

    sensor_t hours = {
        .type = HOUR
        .value = 0
    };

    sensor_t minutes = {
        .type = MINUTES
        .value = 0
    };

    sensor_t seconds = {
        .type = SECONDS
        .value = 0
    };

    
    for (;;) {
        xTimerStart(second_timer, 0);

        seconds.value = timer_count;
        minutes.value = timer_count / 60;
        hours.value = timer_count / 3600;
        
        xQueueSend(qh_hours, &hours, pdMS_TO_TICKS(0));
        xQueueSend(qh_minutes, &minutes, pdMS_TO_TICKS(0)); 
        xQueueSend(qh_seconds, &seconds, pdMS_TO_TICKS(0));

        ESP_LOGI(TAG, "send timer");

        vTaskDelay(pdMS_TO_TICKS(10)); // free up the processor
    }
}
