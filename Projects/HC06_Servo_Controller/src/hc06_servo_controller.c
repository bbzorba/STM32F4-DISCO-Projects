#include "hc06_servo_controller.h"

void move_servo_to_direction(uint8_t command) {
    switch (command)
    {
    case 'l': // Move servo to left position
    case 'L':
        Servo_SetAngle(SERVO_MIN_ANGLE, TIM_9);
        break;
    case 'r': // Move servo to right position
    case 'R': 
        Servo_SetAngle(SERVO_MAX_ANGLE, TIM_9);
        break;
    case 'M': // Move servo to middle position
    case 'm':
        Servo_SetAngle(SERVO_DEFAULT_ANGLE, TIM_9);
        break;
    default:
        break;
    }
}