################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../classlib/date.cpp \
../classlib/time.cpp 

OBJS += \
./classlib/date.o \
./classlib/time.o 

CPP_DEPS += \
./classlib/date.d \
./classlib/time.d 


# Each subdirectory must supply rules for building sources it contributes
classlib/%.o: ../classlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


