#include "i2c.h"

#define i2c_addr 0x76

// Function prototypes
void delay(volatile uint32_t count);

static void sda_pb7_debug_pulse(int pulses, int dly)
{
    // Enable GPIOB clock
    RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Configure PB7 as GPIO output, open-drain with pull-up
    GPIO_B->MODER &= ~(0x3U << (7*2));
    GPIO_B->MODER |=  (0x1U << (7*2));   // output mode
    GPIO_B->OTYPER |= (1U << 7);         // open-drain
    GPIO_B->PUPDR &= ~(0x3U << (7*2));
    GPIO_B->PUPDR |=  (0x1U << (7*2));   // pull-up

    // Release high initially
    GPIO_B->ODR |= (1U << 7);

    for (int i = 0; i < pulses; ++i) {
        // Drive low
        GPIO_B->ODR &= ~(1U << 7);
        delay(dly);
        // Release high
        GPIO_B->ODR |=  (1U << 7);
        delay(dly);
    }
}


//main function
int main(void) {
    // Emit a visible pulse train on PB7 so you can verify analyzer hookup
    sda_pb7_debug_pulse(50, 20000);

    // Initialize I2C1 in standard mode (100kHz)
    I2C_Init(I2C_STANDARD_MODE);

    while (1) {
        // Simple visible transaction: START -> address write -> dummy byte -> STOP
        I2C_Start();
        if (!I2C_SendAddress(i2c_addr, 0)) { // write
            // NACK received, stop and retry after a short delay
            I2C_Stop();
            delay(100000);
            continue;
        }
        I2C_Write(0x15);              // dummy data
        I2C_Stop();
        delay(200000);                 // brief gap between transactions for visibility
    }
}

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}