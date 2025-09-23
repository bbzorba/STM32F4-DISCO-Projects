# Minimal Makefile for STM32F4 Discovery (STM32F407VG)

#PROJECT_DIR = Drivers/UART
PROJECT_DIR = Projects/LED_Blink_cpp

CXX=arm-none-eabi-g++
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
MCU=cortex-m4
RM=del /Q /F

# Flashing configuration
FLASH_ADDR=0x08000000
FLASH_TOOL?=openocd  # options: cubeprog | openocd | stlink
# Sanitize FLASH_TOOL in case of stray spaces from environment or edits
_FLASH_TOOL:=$(strip $(FLASH_TOOL))
CUBE_PROG?=STM32_Programmer_CLI
OPENOCD?=openocd
STFLASH?=st-flash
OPENOCD_SCRIPTS?=
OPENOCD_IF?=interface/stlink.cfg
OPENOCD_TARGET?=target/stm32f4x.cfg

CFLAGS=-mcpu=$(MCU) -mthumb -Wall -O2 -g -DSTM32F407xx -DUSE_HAL_DRIVER \
	-IDrivers/compat_inc \
	-IDrivers/CMSIS \
	-IDrivers/STM32F4xx_HAL_Driver \
	-IDrivers/STM32F4xx_HAL_Driver/Legacy \
	-I$(PROJECT_DIR)/inc \
	-ffunction-sections -fdata-sections

# C++ flags largely mirror C; disable RTTI/exceptions to keep size small
CXXFLAGS=$(CFLAGS) -fno-exceptions -fno-rtti -fno-use-cxa-atexit
	
# Serial monitor settings (Windows PowerShell)
PORT ?=
BAUD ?= 115200
LDFLAGS=-T$(PROJECT_DIR)/linker.ld --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections

# Discover driver directories (exclude HAL, CMSIS, and compat headers)
DRIVER_DIRS_ALL := $(filter-out Drivers/STM32F4xx_HAL_Driver Drivers/CMSIS Drivers/compat_inc,$(wildcard Drivers/*))
# Allow selecting drivers, e.g. `make DRIVERS="UART SPI"`; default is all found
ifeq ($(strip $(DRIVERS)),)
SELECTED_DRIVER_DIRS := $(DRIVER_DIRS_ALL)
else
SELECTED_DRIVER_DIRS := $(addprefix Drivers/,$(DRIVERS))
endif
# Exclude the active project if it lives under Drivers/
FILTERED_DRIVER_DIRS := $(filter-out $(PROJECT_DIR),$(SELECTED_DRIVER_DIRS))
# Driver include paths and sources
DRIVER_INCLUDES := $(foreach d,$(FILTERED_DRIVER_DIRS),-I$(d)/inc)
# Gather driver sources but exclude typical app/CRT units that would conflict
EXTERNAL_SRC_RAW := $(foreach d,$(FILTERED_DRIVER_DIRS),$(wildcard $(d)/src/*.c))
EXTERNAL_EXCLUDE := %/main.c %/startup.c %/system_stm32f4xx.c %/system_%.c
EXTERNAL_SRC := $(filter-out $(EXTERNAL_EXCLUDE),$(EXTERNAL_SRC_RAW))

# Apply driver include paths
CFLAGS += $(DRIVER_INCLUDES)

# Project sources
SRC_C   := $(wildcard $(PROJECT_DIR)/src/*.c)
SRC_CPP := $(wildcard $(PROJECT_DIR)/src/*.cpp)
SRC := $(SRC_C) $(SRC_CPP)

# If the project uses HAL (e.g., VCP), add a minimal set of HAL sources
HAL_SRC := \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pcd.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_ll_usb.c

# Automatically include HAL sources when project includes vcp.c (USB)
ifneq (,$(findstring vcp.c,$(notdir $(SRC))))
SRC += $(HAL_SRC)
endif
OBJ=$(SRC_C:.c=.o) $(SRC_CPP:.cpp=.o) $(EXTERNAL_SRC:.c=.o)
TARGET=$(PROJECT_DIR)/main

# Use C++ linker if there are any C++ sources, else C linker
LINKER := $(if $(strip $(SRC_CPP)),$(CXX),$(CC))
TARGET=$(PROJECT_DIR)/main

# Pattern rules
%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: all build run clean flash flash_openocd flash_stlink flash_cubeprog

all: build
build: $(OBJ)
	$(LINKER) $(CFLAGS) $(OBJ) -Wl,-Map=$(TARGET).map -o $(TARGET).elf $(LDFLAGS) $(EXTRA_LDFLAGS)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

run: build

flash: build
ifeq ($(_FLASH_TOOL),cubeprog)
	"$(CUBE_PROG)" -c port=SWD -halt -d $(TARGET).bin $(FLASH_ADDR) -rst
else ifeq ($(_FLASH_TOOL),openocd)
	"$(OPENOCD)" $(if $(OPENOCD_SCRIPTS),-s "$(OPENOCD_SCRIPTS)") -f $(OPENOCD_IF) -f $(OPENOCD_TARGET) -c "program $(TARGET).elf verify reset exit"
else ifeq ($(_FLASH_TOOL),stlink)
	"$(STFLASH)" --reset write $(TARGET).bin $(FLASH_ADDR)
else
	@echo Unknown FLASH_TOOL=$(_FLASH_TOOL). Use cubeprog, openocd, or stlink.
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

.PHONY: com-list
com-list:
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "Get-CimInstance Win32_SerialPort | Select-Object Name,DeviceID | Format-Table -AutoSize"
else
	@echo "Use: ls /dev/tty*"
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