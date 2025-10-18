#include "hc06_servo_controller.h"

void move_servo_to_direction(Servo *servoMotor, uint8_t command) {
    switch (command)
    {
    case 'l': // Move servo to left position
    case 'L':
        Servo_SetAngle(servoMotor, SERVO_MIN_ANGLE);
        break;
    case 'r': // Move servo to right position
    case 'R': 
        Servo_SetAngle(servoMotor, SERVO_MAX_ANGLE);
        break;
    case 'M': // Move servo to middle position
    case 'm':
        Servo_SetAngle(servoMotor, SERVO_DEFAULT_ANGLE);
        break;
    default:
        break;
    }
}