#include "task/timer.h"

static const char *TAG = "timer";

void task_timer(void *arg){
    float start_time = esp_timer_get_time()/1000000;
    
    sensor_t hours = {
        .type = HOURS,
        .value = 0.0
    };

    sensor_t minutes = {
        .type = MINUTES,
        .value = 0.0
    };

    sensor_t seconds = {
        .type = SECONDS,
        .value = 0.0
    };

    
    for (;;) {
        float current_time = esp_timer_get_time()/1000000;

        int time = (int) (current_time - start_time);

        seconds.value = (float) time;
        minutes.value = (float) (time / 60);
        hours.value = (float) (time / 3600);
        
        xQueueSend(qh_hours, &hours, pdMS_TO_TICKS(0));
        xQueueSend(qh_minutes, &minutes, pdMS_TO_TICKS(0)); 
        xQueueSend(qh_seconds, &seconds, pdMS_TO_TICKS(0));

        ESP_LOGI(TAG, "send timer, hours: %f, minutes: %f, seconds: %f", hours.value, minutes.value, seconds.value);

        vTaskDelay(pdMS_TO_TICKS(1000)); // free up the processor
    }
}
