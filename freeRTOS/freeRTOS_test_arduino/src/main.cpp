#include <Arduino.h>



#define BLINK_GPIO (gpio_num_t)2


void hello_task(void *pvParameter)
{
	while(1)
	{
	    Serial.printf("Hello world!\n");
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
        Serial.printf("LED OFF\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        Serial.printf("LED ON\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#define TASK_NAME_LEN 16

void monitor_task(void *pvParameter)
{
    // Get the number of tasks in the system
    while(1){
        UBaseType_t numTasks = uxTaskGetNumberOfTasks();

        // Allocate memory for the task status array
        TaskStatus_t *taskStatusArray = (TaskStatus_t *)pvPortMalloc(numTasks * sizeof(TaskStatus_t));

        // Get the task status array
        UBaseType_t numTasksUpdated = uxTaskGetSystemState(taskStatusArray, numTasks, NULL);

        Serial.printf("Number of tasks: %u\n", numTasksUpdated);

        // Iterate through the task status array
        for (UBaseType_t i = 0; i < numTasksUpdated; i++)
        {
            TaskStatus_t taskStatus = taskStatusArray[i];
            Serial.printf("Task Name: %s, State: %d, Priority: %u, Run Time Counter: %lu\n",
                   taskStatus.pcTaskName,
                   taskStatus.eCurrentState,
                   taskStatus.uxCurrentPriority,
                   taskStatus.ulRunTimeCounter);


        }

        // Free the memory allocated for the task status array
        vPortFree(taskStatusArray);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    }
}

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB port only
    }
    
    // Initialize GPIO for LED
    //gpio_pad_select_gpio(BLINK_GPIO);
    //gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void loop()
{
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 2048,NULL,5,NULL );
    xTaskCreate(&monitor_task, "monitor_task", 2048,NULL,5,NULL );
}
