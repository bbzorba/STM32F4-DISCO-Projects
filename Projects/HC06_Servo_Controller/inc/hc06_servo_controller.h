// Dedicated header for HC-06 Servo Controller
#ifndef HC06_SERVO_CONTROLLER_H
#define HC06_SERVO_CONTROLLER_H

#include "../../Projects/Servo_Motor/inc/servo.h"
#include "../../Projects/HC06_Bluetooth/inc/hc06.h"
#include <stdint.h>

void move_servo_to_direction(uint8_t command);

#endif // HC06_SERVO_CONTROLLER_H