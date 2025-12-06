#include "../inc/lsm303dlhc_accel.h"

void delay_cycles(volatile uint32_t c)
{
    while (c--) __asm__("nop");
}

void LSM303DLHC_Init(I2C_HandleType *hi2c)
{
    // Don't reinitialize I2C - it's already done in main
    
    // Configure accelerometer: Normal mode, all axes enabled, 10Hz
    I2C_Start(hi2c);
    if (I2C_SendAddress(hi2c, LSM303DLHC_ADDR, 0)) {
        I2C_Write(hi2c, 0x20); // CTRL_REG1
        I2C_Write(hi2c, 0x27); // Normal mode, XYZ enabled, 10Hz
    }
    I2C_Stop(hi2c);
    delay_cycles(10000);
    
    // Configure accelerometer scale: +/-2g
    I2C_Start(hi2c);
    if (I2C_SendAddress(hi2c, LSM303DLHC_ADDR, 0)) {
        I2C_Write(hi2c, 0x23); // CTRL_REG4
        I2C_Write(hi2c, 0x00); // +/-2g, high resolution mode
    }
    I2C_Stop(hi2c);
    delay_cycles(10000);
}

int read_accel(I2C_HandleType *hi2c, uint8_t address)
{
    // Check if device is still responding
    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return 0xFFFF; // Device not responding
    }
    I2C_Stop(hi2c);
    
    // Small delay
    delay_cycles(1000);

    // Read status register to check if new data is available
    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }
    I2C_Write(hi2c, 0x27); // STATUS_REG_A
    I2C_Restart(hi2c);
    if (!I2C_SendAddress(hi2c, address, 1)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }
    I2C_DisableAck(hi2c);
    while (!(hi2c->regs->SR1 & I2C_SR1_RXNE));
    uint8_t status = (uint8_t)hi2c->regs->DR;
    I2C_Stop(hi2c);
    I2C_EnableAck(hi2c);
    
    // Check if new data is available (bit 3 = ZYXDA - XYZ-axis new data available)
    if (!(status & 0x08)) {
        // No new data available, wait a bit and try again
        delay_cycles(5000); // Wait ~0.3ms at 168MHz
        
        // Try reading status again
        I2C_Start(hi2c);
        if (!I2C_SendAddress(hi2c, address, 0)) {
            I2C_Stop(hi2c);
            return 0xFFFF;
        }
        I2C_Write(hi2c, 0x27); // STATUS_REG_A
        I2C_Restart(hi2c);
        if (!I2C_SendAddress(hi2c, address, 1)) {
            I2C_Stop(hi2c);
            return 0xFFFF;
        }
        I2C_DisableAck(hi2c);
        while (!(hi2c->regs->SR1 & I2C_SR1_RXNE));
        status = (uint8_t)hi2c->regs->DR;
        I2C_Stop(hi2c);
        I2C_EnableAck(hi2c);
        
        // If still no new data, return previous error or read anyway
        if (!(status & 0x08)) {
            // Could return error or proceed to read anyway for debugging
            // For now, let's read anyway to see if we get valid data
        }
    }
    
    delay_cycles(1000);
    
    // Read acceleration data (6 bytes: OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L, OUT_Z_H)
    uint8_t data[6];
    I2C_Start(hi2c);
    if (!I2C_SendAddress(hi2c, address, 0)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }
    I2C_Write(hi2c, 0x28 | 0x80); // OUT_X_L with auto-increment
    I2C_Restart(hi2c);
    if (!I2C_SendAddress(hi2c, address, 1)) {
        I2C_Stop(hi2c);
        return 0xFFFF;
    }
    
    for (int i = 0; i < 6; i++) {
        if (i == 5) {
            I2C_DisableAck(hi2c); // NACK on last byte
        } else {
            I2C_EnableAck(hi2c);
        }
        while (!(hi2c->regs->SR1 & I2C_SR1_RXNE));
        data[i] = (uint8_t)hi2c->regs->DR;
    }
    I2C_Stop(hi2c);
    I2C_EnableAck(hi2c);

    // Combine bytes into 16-bit signed integers (LSM303DLHC is little-endian for accelerometer)
    int16_t x = (int16_t)((data[1] << 8) | data[0]);
    int16_t y = (int16_t)((data[3] << 8) | data[2]);
    int16_t z = (int16_t)((data[5] << 8) | data[4]);

    // For simplicity, return the magnitude of the acceleration vector
    // Use absolute values to avoid overflow in multiplication
    int32_t x_abs = (x < 0) ? -x : x;
    int32_t y_abs = (y < 0) ? -y : y;
    int32_t z_abs = (z < 0) ? -z : z;
    
    // Return a simpler metric: sum of absolute values
    int32_t magnitude = x_abs + y_abs + z_abs;
    return (int)magnitude;
}