################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../cc32xxs_nortos.cmd 

SYSCFG_SRCS += \
../gpiointerrupt.syscfg \
../image.syscfg 

C_SRCS += \
../callbacks.c \
../gpiointerrupt.c \
./syscfg/ti_drivers_config.c \
../init.c \
../main_nortos.c \
../utils.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg \
./syscfg 

C_DEPS += \
./callbacks.d \
./gpiointerrupt.d \
./syscfg/ti_drivers_config.d \
./init.d \
./main_nortos.d \
./utils.d 

OBJS += \
./callbacks.o \
./gpiointerrupt.o \
./syscfg/ti_drivers_config.o \
./init.o \
./main_nortos.o \
./utils.o 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/syscfg_c.rov.xs \
./syscfg/ti_drivers_net_wifi_config.json 

GEN_MISC_DIRS__QUOTED += \
"syscfg" \
"syscfg" 

OBJS__QUOTED += \
"callbacks.o" \
"gpiointerrupt.o" \
"syscfg\ti_drivers_config.o" \
"init.o" \
"main_nortos.o" \
"utils.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\ti_drivers_net_wifi_config.json" 

C_DEPS__QUOTED += \
"callbacks.d" \
"gpiointerrupt.d" \
"syscfg\ti_drivers_config.d" \
"init.d" \
"main_nortos.d" \
"utils.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../callbacks.c" \
"../gpiointerrupt.c" \
"./syscfg/ti_drivers_config.c" \
"../init.c" \
"../main_nortos.c" \
"../utils.c" 

SYSCFG_SRCS__QUOTED += \
"../gpiointerrupt.syscfg" \
"../image.syscfg" 


