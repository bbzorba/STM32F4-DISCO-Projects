# Minimal Makefile for STM32F4 Discovery (STM32F407VG)

#PROJECT_DIR = Drivers/UART
#PROJECT_DIR = Drivers/UART_cpp
#PROJECT_DIR = Drivers/GPIO
#PROJECT_DIR = Drivers/GPIO_cpp
#PROJECT_DIR = Drivers/PWM
#PROJECT_DIR = Drivers/PWM_cpp
#PROJECT_DIR = Drivers/SysTick
#PROJECT_DIR = Drivers/SysTick_cpp
#PROJECT_DIR = Projects/LED_Blink
#PROJECT_DIR = Projects/LED_Blink_cpp
#PROJECT_DIR = Projects/Servo_Motor
#PROJECT_DIR = Projects/Servo_Motor_cpp
#PROJECT_DIR = Projects/HC06_Bluetooth
#PROJECT_DIR = Projects/HC06_Bluetooth_cpp
#PROJECT_DIR = Projects/HC06_Servo_Controller
PROJECT_DIR = Projects/HC06_Servo_Controller_cpp

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

# Optional external driver selection -------------------------------------------------
# By default (when DRIVERS is empty) we build ONLY the selected PROJECT sources.
# To pull in driver code under Drivers/<Name>, invoke:
#   make DRIVERS="UART I2C"   (names are directory basenames)
# We still exclude HAL, CMSIS, compat headers, and the active project if it lives
# under Drivers/.
ifeq ($(strip $(DRIVERS)),)
FILTERED_DRIVER_DIRS :=
else
DRIVER_DIRS_REQUESTED := $(addprefix Drivers/,$(DRIVERS))
# Filter out directories that do not exist to avoid warnings and dedupe
DRIVER_DIRS_EXISTING := $(sort $(wildcard $(DRIVER_DIRS_REQUESTED)))
FILTERED_DRIVER_DIRS := $(sort $(filter-out Drivers/STM32F4xx_HAL_Driver Drivers/CMSIS Drivers/compat_inc $(PROJECT_DIR),$(DRIVER_DIRS_EXISTING)))
endif

DRIVER_INCLUDES := $(foreach d,$(FILTERED_DRIVER_DIRS),-I$(d)/inc)
EXTERNAL_SRC_C_RAW   := $(foreach d,$(FILTERED_DRIVER_DIRS),$(wildcard $(d)/src/*.c))
EXTERNAL_SRC_CPP_RAW := $(foreach d,$(FILTERED_DRIVER_DIRS),$(wildcard $(d)/src/*.cpp))
EXTERNAL_EXCLUDE_C   := %/main.c %/startup.c %/system_stm32f4xx.c %/system_%.c
EXTERNAL_EXCLUDE_CPP := %/main.cpp %/startup.cpp %/system_%.cpp
EXTERNAL_SRC_C   := $(sort $(filter-out $(EXTERNAL_EXCLUDE_C),$(EXTERNAL_SRC_C_RAW)))
EXTERNAL_SRC_CPP := $(sort $(filter-out $(EXTERNAL_EXCLUDE_CPP),$(EXTERNAL_SRC_CPP_RAW)))

CFLAGS += $(DRIVER_INCLUDES)

# Project sources
SRC_C   := $(wildcard $(PROJECT_DIR)/src/*.c)
SRC_CPP := $(wildcard $(PROJECT_DIR)/src/*.cpp)
SRC := $(SRC_C) $(SRC_CPP)

# If the project uses HAL, add a minimal set of HAL sources
HAL_SRC := \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pcd.c \
	Drivers/STM32F4xx_HAL_Driver/stm32f4xx_ll_usb.c

GPIO_SRC_C := Drivers/GPIO/src/gpio.c
GPIO_SRC_CPP := Drivers/GPIO_cpp/src/gpio.cpp

UART_SRC_C := Drivers/UART/src/uart.c
UART_SRC_CPP := Drivers/UART_cpp/src/uart.cpp

PWM_SRC_C := Drivers/PWM/src/pwm.c
PWM_SRC_CPP := Drivers/PWM_cpp/src/pwm.cpp

HC_06_SRC_C := Projects/HC06_Bluetooth/src/hc06.c
HC_06_SRC_CPP := Projects/HC06_Bluetooth_cpp/src/hc06.cpp

Servo_MOTOR_SRC_C := Projects/Servo_Motor/src/servo.c
Servo_MOTOR_SRC_CPP := Projects/Servo_Motor_cpp/src/servo.cpp

# Automatically include GPIO library when project includes the following source files
ifneq (,$(filter systick.c hc06.c pwm.c servo.c,$(notdir $(SRC))))
# Important: OBJ is derived from SRC_C/SRC_CPP (not SRC). Only add if not already present.
SRC_C += $(filter-out $(SRC_C),$(GPIO_SRC_C))
CFLAGS += -IDrivers/GPIO/inc
SRC_C += $(filter-out $(SRC_C),$(UART_SRC_C))
CFLAGS += -IDrivers/UART/inc
SRC_C += $(filter-out $(SRC_C),$(PWM_SRC_C))
CFLAGS += -IDrivers/PWM/inc
endif

# Automatically include GPIO_cpp, UART_cpp, and PWM_cpp libraries when project includes the following source files
ifneq (,$(filter systick.cpp hc06.cpp pwm.cpp servo.cpp,$(notdir $(SRC))))
SRC_CPP += $(filter-out $(SRC_CPP),$(GPIO_SRC_CPP))
CFLAGS += -IDrivers/GPIO_cpp/inc
SRC_CPP += $(filter-out $(SRC_CPP),$(UART_SRC_CPP))
CFLAGS += -IDrivers/UART_cpp/inc
SRC_CPP += $(filter-out $(SRC_CPP),$(PWM_SRC_CPP))
CFLAGS += -IDrivers/PWM_cpp/inc
# Also link the C PWM driver to satisfy free-function usages like Timer_Init/Configure_PWM from C++ code
SRC_C += $(filter-out $(SRC_C),$(PWM_SRC_C))
CFLAGS += -IDrivers/PWM/inc
endif

# Project-specific wiring for C++ servo/controller projects to ensure PWM_cpp is compiled
ifeq ($(PROJECT_DIR),Projects/Servo_Motor_cpp)
SRC_CPP += $(filter-out $(SRC_CPP),$(GPIO_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(UART_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(PWM_SRC_CPP))
SRC_C   += $(filter-out $(SRC_C),$(PWM_SRC_C))
CFLAGS  += -IDrivers/GPIO_cpp/inc -IDrivers/UART_cpp/inc -IDrivers/PWM_cpp/inc -IDrivers/PWM/inc
endif

ifeq ($(PROJECT_DIR),Projects/HC06_Servo_Controller_cpp)
SRC_CPP += $(filter-out $(SRC_CPP),$(GPIO_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(UART_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(PWM_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(HC_06_SRC_CPP))
SRC_CPP += $(filter-out $(SRC_CPP),$(Servo_MOTOR_SRC_CPP))
CFLAGS  += -IDrivers/GPIO_cpp/inc -IDrivers/UART_cpp/inc -IDrivers/PWM_cpp/inc -IDrivers/PWM/inc
endif
OBJ_UNSORTED=$(SRC_C:.c=.o) $(SRC_CPP:.cpp=.o) $(EXTERNAL_SRC_C:.c=.o) $(EXTERNAL_SRC_CPP:.cpp=.o)
OBJ=$(sort $(OBJ_UNSORTED))
TARGET=$(PROJECT_DIR)/main

# Use C++ linker if there are any C++ sources (project or external driver), else C linker
ifneq (,$(strip $(SRC_CPP) $(EXTERNAL_SRC_CPP)))
LINKER := $(CXX)
else
LINKER := $(CC)
endif
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

# Project-specific wiring for HC06_Servo_Controller
ifeq ($(PROJECT_DIR),Projects/HC06_Servo_Controller)
# Include required drivers and servo/hc06 helpers
SRC_C += $(filter-out $(SRC_C),$(GPIO_SRC_C))
SRC_C += $(filter-out $(SRC_C),$(UART_SRC_C))
SRC_C += $(filter-out $(SRC_C),$(PWM_SRC_C))
SRC_C += $(filter-out $(SRC_C),Projects/Servo_Motor/src/servo.c)
SRC_C += $(filter-out $(SRC_C),Projects/HC06_Bluetooth/src/hc06.c)
CFLAGS += -IDrivers/GPIO/inc -IDrivers/UART/inc -IDrivers/PWM/inc -IProjects/Servo_Motor/inc -IProjects/HC06_Bluetooth/inc
endif