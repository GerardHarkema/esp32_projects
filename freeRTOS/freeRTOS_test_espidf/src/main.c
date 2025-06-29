/*
* This project file the use of FreeRTOS tasks and command console for monitoring task states and run-time statistics.
* It includes a simple command console that allows users to view task states and run-time statistics.   
* This file creates a simple task that prints "Hello world" every 2 seconds.
* The command console allows users to interact with the system by entering commands.
* The task list and run-time statistics can be viewed by entering 't' and 'r' respectively.
* The task list shows the state of each FreeRTOS task, including its name, state, priority, stack size, and number of tasks.
* The run-time statistics show how much processing time each FreeRTOS task has used, including the absolute time and the time in milliseconds.
* The command console is implemented in the CommandConsole library, and the heartbeat task is implemented in the Hartbeat library.
* The GPIO pin for the heartbeat LED can be configured using the HARTBEAT_LED_PIN macro, which defaults to GPIO 2 but can be overridden by build flags.
* The FreeRTOS configuration options for generating run-time statistics and using the trace facility are set in the sdkconfig file.
* Author: Gerard Harkema
* Date: 2023-10-01
* License: Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
* https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <string.h>

#include "CommandConsole.h"
#include "Hartbeat.h"


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


void app_main()
{
    startHartbeatTask();
    startCommandConsoleTask();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);

    while(1)
    {
        // The main task does nothing, just waits
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
