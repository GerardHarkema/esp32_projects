/*
* This project file implements a simple heartbeat task that blinks an LED on a specified GPIO pin.
* The LED blinks every second to indicate that the system is running.
* The GPIO pin can be configured using the HARTBEAT_LED_PIN macro, which defaults to GPIO 2 but can be overridden by build flags.
* Author: Gerard Harkema
* Date: 2023-10-01
* License: Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
* https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#include "Hartbeat.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#ifndef HARTBEAT_LED_PIN
#define HARTBEAT_LED_PIN 2      // Default GPIO pin for the heartbeat LED, can be overridden by build flags
#endif

void hartbeat_task(void *pvParameter)
{
    /* Set the GPIO as a push/pull output */
    // Configure the GPIO pin
    gpio_reset_pin(HARTBEAT_LED_PIN);
    gpio_set_direction(HARTBEAT_LED_PIN, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        //printf("LED OFF\n");
        gpio_set_level(HARTBEAT_LED_PIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        //printf("LED ON\n");
        gpio_set_level(HARTBEAT_LED_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void startHartbeatTask(void){
    xTaskCreate(&hartbeat_task, "hartbeat", 2048,NULL,5,NULL );
}