# Minimal Makefile for STM32F4 Discovery (STM32F407VG)

PROJECT_DIR = Projects/LED_Blink

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
MCU=cortex-m4

CFLAGS=-mcpu=$(MCU) -mthumb -Wall -O2 -g -nostdlib -DSTM32F407xx -DSTM32F40XX \
  -I$(PROJECT_DIR)/Inc -IDrivers/HAL/Inc -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Core/Include
LDFLAGS=-T$(PROJECT_DIR)/linker.ld

SRC=$(PROJECT_DIR)/Src/main.c
STARTUP=$(PROJECT_DIR)/src/startup.c
OBJ=$(SRC:.c=.o) $(STARTUP:.c=.o)
TARGET=$(PROJECT_DIR)/main

# Pattern rule: compile .s files with arm-none-eabi-gcc
%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all build run clean

all: build
build: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).elf $(LDFLAGS)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

run: build

clean:
	@if [ -f "$(OBJ)" ]; then rm -f $(OBJ); fi
	@if [ -f "$(TARGET).elf" ]; then rm -f $(TARGET).elf; fi
	@if [ -f "$(TARGET).hex" ]; then rm -f $(TARGET).hex; fi
	@if [ -f "$(TARGET).bin" ]; then rm -f $(TARGET).bin; fi
	@if [ -f "$(OBJ)" ]; then del /Q $(OBJ) 2>nul || exit 0; fi
	@if [ -f "$(TARGET).elf" ]; then del /Q $(TARGET).elf 2>nul || exit 0; fi
	@if [ -f "$(TARGET).hex" ]; then del /Q $(TARGET).hex 2>nul || exit 0; fi
	@if [ -f "$(TARGET).bin" ]; then del /Q $(TARGET).bin 2>nul || exit 0; fi

# End of Makefile