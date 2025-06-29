/*
* This project file interfaces a simple heartbeat task that blinks an LED on a specified GPIO pin.
* The LED blinks every second to indicate that the system is running.
* The GPIO pin can be configured using the HARTBEAT_LED_PIN macro, which defaults to GPIO 2 but can be overridden by build flags.
* Author: Gerard Harkema
* Date: 2023-10-01
* License: Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
* https://creativecommons.org/licenses/by-nc-sa/4.0/
*/
#ifndef __HARTBEAT_H__
#define __HARTBEAT_H__
void startHartbeatTask(void);
#endif