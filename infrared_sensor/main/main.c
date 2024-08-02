#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include "esp_log.h"

#define pin_sensor_ifr  GPIO_NUM_27
#define TAG "ISR"
#define queue_size 10
#define task_size (2048)
#define DEBOUNCE_DELAY_MS 100 

TickType_t last_interrupt_time = 0;
QueueHandle_t gpio_evt_queue = NULL;

void pin_config(void){
    esp_rom_gpio_pad_select_gpio(pin_sensor_ifr);
    gpio_set_direction(pin_sensor_ifr, GPIO_MODE_INPUT);
    gpio_pullup_dis(pin_sensor_ifr);
    gpio_pulldown_dis(pin_sensor_ifr);
    gpio_set_intr_type(pin_sensor_ifr, GPIO_INTR_NEGEDGE); //GPIO interrupt type: falling edge
}

void IRAM_ATTR sensor_ifr_handler(void* arg) {
    uint32_t io_num = (uint32_t)arg; // Using the passed value as an argument
    TickType_t current_time = xTaskGetTickCount();

    if ((current_time - last_interrupt_time) * portTICK_PERIOD_MS > DEBOUNCE_DELAY_MS) {
        last_interrupt_time = current_time;  // Updates the time of the last interruption
        BaseType_t higher_priority_task_woken = pdFALSE;
        xQueueSendFromISR(gpio_evt_queue, &io_num, NULL); 
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

void gpio_task(void* arg) {
    uint32_t io_num;
    while (1) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // Processar o evento do GPIO
            ESP_LOGI(TAG,"Detected");
        }
    }
}

void app_main(void){
    pin_config();
    gpio_evt_queue = xQueueCreate(queue_size, sizeof(uint32_t));
    esp_err_t ret = gpio_install_isr_service(0);

    if (gpio_evt_queue == NULL) {
        printf("Failed to create queue");
        return;
    }

    if (ret != ESP_OK) {
        printf("Failed to install ISR service: %s\n", esp_err_to_name(ret));
        return;
    }

    gpio_isr_handler_add(pin_sensor_ifr, sensor_ifr_handler, (void*) pin_sensor_ifr);
    xTaskCreate(gpio_task, "gpio_task", task_size, NULL, 10, NULL);
    vTaskDelay(pdMS_TO_TICKS(10000));
}


