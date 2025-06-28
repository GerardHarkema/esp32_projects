// This code is for an ESP-IDF project using FreeRTOS to create three tasks:
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <string.h>

#define BLINK_GPIO 2


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

void monitor_task(void *pvParameter)
{
    while(1){
#if 0
        // Get the number of tasks in the system
        UBaseType_t numTasks = uxTaskGetNumberOfTasks();

        // Allocate memory for the task status array
        TaskStatus_t *taskStatusArray = (TaskStatus_t *)pvPortMalloc(numTasks * sizeof(TaskStatus_t));

        // Get the task status array
        UBaseType_t numTasksUpdated = uxTaskGetSystemState(taskStatusArray, numTasks, NULL);

        printf("Number of tasks: %u\n", numTasksUpdated);

        // Iterate through the task status array
        for (UBaseType_t i = 0; i < numTasksUpdated; i++)
        {
            TaskStatus_t taskStatus = taskStatusArray[i];
            printf("Task Name: %s, State: %d, Priority: %u, Run Time Counter: %lu\n",
                   taskStatus.pcTaskName,
                   taskStatus.eCurrentState,
                   taskStatus.uxCurrentPriority,
                   taskStatus.ulRunTimeCounter);


        }
        // Free the memory allocated for the task status array
        vPortFree(taskStatusArray);

#else
        vTaskList(ptrTaskList);
        printf("******************************************\n");
        printf("Task          State   Prio    Stack    Num\n"); 
        printf("******************************************\n");
        printf(ptrTaskList);
        printf("******************************************\n");

#endif    
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 2048,NULL,5,NULL );
    xTaskCreate(&monitor_task, "monitor_task", 2048,NULL,5,NULL );
}
