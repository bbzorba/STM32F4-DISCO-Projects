# Minimal Makefile for STM32F4 Discovery (STM32F407VG)

PROJECT_DIR = Drivers/UART

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
MCU=cortex-m4
RM=del /Q /F

# Flashing configuration
FLASH_ADDR=0x08000000
FLASH_TOOL?=cubeprog  # options: cubeprog | openocd | stlink
CUBE_PROG?=STM32_Programmer_CLI
OPENOCD?=openocd
STFLASH?=st-flash
OPENOCD_SCRIPTS?=
OPENOCD_IF?=interface/stlink.cfg
OPENOCD_TARGET?=target/stm32f4x.cfg

CFLAGS=-mcpu=$(MCU) -mthumb -Wall -O2 -g -DSTM32F407xx \
	-IDrivers/compat_inc \
	-I$(PROJECT_DIR)/inc \
	-IDrivers/STM32F4xx_HAL_Driver \
	-IDrivers/CMSIS
	
# Serial monitor settings (Windows PowerShell)
PORT ?=
BAUD ?= 115200
LDFLAGS=-T$(PROJECT_DIR)/linker.ld --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections

SRC=$(PROJECT_DIR)/src/main.c \
	$(PROJECT_DIR)/src/uart.c \
	$(PROJECT_DIR)/src/system_stm32f4xx.c \
	$(PROJECT_DIR)/src/stubs.c
STARTUP=$(PROJECT_DIR)/src/startup.c
OBJ=$(SRC:.c=.o) $(STARTUP:.c=.o)
TARGET=$(PROJECT_DIR)/main

# Pattern rule: compile .s files with arm-none-eabi-gcc
%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all build run clean flash flash_openocd flash_stlink flash_cubeprog

all: build
build: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -Wl,-Map=$(TARGET).map -o $(TARGET).elf $(LDFLAGS)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

run: build

flash: build
ifeq ($(FLASH_TOOL),cubeprog)
	"$(CUBE_PROG)" -c port=SWD -halt -d $(TARGET).bin $(FLASH_ADDR) -rst
else ifeq ($(FLASH_TOOL),openocd)
	"$(OPENOCD)" $(if $(OPENOCD_SCRIPTS),-s "$(OPENOCD_SCRIPTS)") -f $(OPENOCD_IF) -f $(OPENOCD_TARGET) -c "program $(TARGET).elf verify reset exit"
else ifeq ($(FLASH_TOOL),stlink)
	"$(STFLASH)" --reset write $(TARGET).bin $(FLASH_ADDR)
else
	@echo Unknown FLASH_TOOL=$(FLASH_TOOL). Use cubeprog, openocd, or stlink.
	@exit 1
endif

flash_cubeprog: FLASH_TOOL=cubeprog
flash_cubeprog: build
	"$(CUBE_PROG)" -c port=SWD -halt -d $(TARGET).bin $(FLASH_ADDR) -rst

flash_openocd: FLASH_TOOL=openocd
flash_openocd: build
	"$(OPENOCD)" $(if $(OPENOCD_SCRIPTS),-s "$(OPENOCD_SCRIPTS)") -f $(OPENOCD_IF) -f $(OPENOCD_TARGET) -c "program $(TARGET).elf verify reset exit"


# Open a simple serial monitor with PowerShell
.PHONY: monitor
monitor:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -ExecutionPolicy Bypass -File "tools/monitor.ps1" $(if $(PORT),-ComPort $(PORT),) -Baud $(BAUD)
else
	@echo "This monitor target is Windows-specific (PowerShell)."
endif

# Build, flash, then start monitor automatically
.PHONY: flashmonitor-auto
flashmonitor-auto: all flash monitor

flash_stlink: FLASH_TOOL=stlink
flash_stlink: build
	"$(STFLASH)" --reset write $(TARGET).bin $(FLASH_ADDR)

clean:
	-$(RM) $(OBJ) 2>nul || exit 0
	-$(RM) $(TARGET).elf 2>nul || exit 0
	-$(RM) $(TARGET).hex 2>nul || exit 0
	-$(RM) $(TARGET).bin 2>nul || exit 0
	-$(RM) $(TARGET).map 2>nul || exit 0

# End of Makefile