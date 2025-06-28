// This code is for an ESP-IDF project using FreeRTOS to create three tasks:
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <string.h>

#define BLINK_GPIO 2


/*
set the following in sdkconfig.[environment, see plaformio.ini]:

CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
*/


void hello_task(void *pvParameter)
{
    int count = 0;
	while(1)
	{
	    printf("Hello world %i!\n", count++);
	    vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}
void blinky(void *pvParameter)
{
    //gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    // Configure the GPIO pin
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        //printf("LED OFF\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        //printf("LED ON\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#define TASK_NAME_LEN 16
char ptrTaskList[1024] = {0}; // Buffer to hold the task list

void print_menu()
{
    printf("******************************************\n");
    printf("Press 't' --> task-stats: Displays a table showing the state of each FreeRTOS task\n");
    printf("Press 'r' --> run-time-stats: Displays a table showing how much processing time each FreeRTOS task has used\n");
    printf("******************************************\n");
}

void monitor_task(void *pvParameter)
{
    print_menu();
    while(1){
        char c = getchar();
        switch (c)
        {
        case 't':
            vTaskList(ptrTaskList);
            printf("******************************************\n");
            printf("Task          State   Prio    Stack    Num\n"); 
            printf("******************************************\n");
            printf(ptrTaskList);
            printf("******************************************\n");
            print_menu();
            break;
        case 'r':
            vTaskGetRunTimeStats(ptrTaskList);
            printf("******************************************\n");
            printf("Task            Abs Time(ms)    Time(ms)\n");
            printf("******************************************\n");
            printf(ptrTaskList);
            printf("******************************************\n");
            print_menu();
            break;
        case 0x0a: // Enter key
            print_menu();
        break;
        default:
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 2048,NULL,5,NULL );
    xTaskCreate(&monitor_task, "monitor_task", 2048,NULL,5,NULL );
    while(1)
    {
        // The main task does nothing, just waits
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
