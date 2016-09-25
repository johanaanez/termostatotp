################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../client.c \
../dateTimee.c \
../list.c \
../package.c \
../server.c \
../socket.c \
../tp.c 

OBJS += \
./client.o \
./dateTimee.o \
./list.o \
./package.o \
./server.o \
./socket.o \
./tp.o 

C_DEPS += \
./client.d \
./dateTimee.d \
./list.d \
./package.d \
./server.d \
./socket.d \
./tp.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


