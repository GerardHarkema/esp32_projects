/*
* This project file the use of FreeRTOS tasks and command console for monitoring task states and run-time statistics.
* It includes a simple command console that allows users to view task states and run-time statistics.
* Author: Gerard Harkema
* Date: 2023-10-01
* License: Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
* https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#include "CommandConsole.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

/*
set the following in sdkconfig.[environment, see plaformio.ini]:

CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
*/


void show_command_menu()
{
    printf("******************************************\n");
    printf("Press 't' --> task-stats: Displays a table showing the state of each FreeRTOS task\n");
    printf("Press 'r' --> run-time-stats: Displays a table showing how much processing time each FreeRTOS task has used\n");
    printf("******************************************\n");
}


char ptrTaskList[1024] = {0}; // Buffer to hold the task list


void command_console_task(void *pvParameter)
{
    show_command_menu();
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
            show_command_menu();
            break;
        case 'r':
            vTaskGetRunTimeStats(ptrTaskList);
            printf("******************************************\n");
            printf("Task            Abs Time(ms)    Time(ms)\n");
            printf("******************************************\n");
            printf(ptrTaskList);
            printf("******************************************\n");
            show_command_menu();
            break;
        case 0x0a: // Enter key
            show_command_menu();
            break;
        default:
            break;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void startCommandConsoleTask(void){
    xTaskCreate(&command_console_task, "command_console", 2048,NULL,5,NULL );
}