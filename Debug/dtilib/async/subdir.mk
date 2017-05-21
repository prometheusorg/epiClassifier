################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dtilib/async/async.cpp \
../dtilib/async/events.cpp 

OBJS += \
./dtilib/async/async.o \
./dtilib/async/events.o 

CPP_DEPS += \
./dtilib/async/async.d \
./dtilib/async/events.d 


# Each subdirectory must supply rules for building sources it contributes
dtilib/async/%.o: ../dtilib/async/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


