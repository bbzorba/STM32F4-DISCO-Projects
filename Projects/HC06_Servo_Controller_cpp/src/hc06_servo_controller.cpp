#include "hc06_servo_controller.h"

void move_servo_to_direction(Servo *servo, uint8_t command) {
    switch (command)
    {
    case 'l': // Move servo to left position
    case 'L':
        servo->SetAngle(SERVO_MIN_ANGLE);
        break;
    case 'r': // Move servo to right position
    case 'R':
        servo->SetAngle(SERVO_MAX_ANGLE);
        break;
    case 'M': // Move servo to middle position
    case 'm':
        servo->SetAngle(SERVO_DEFAULT_ANGLE);
        break;
    default:
        break;
    }
}