################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nsdatabaseLus/nsSQL.cpp 

O_SRCS += \
../nsdatabaseLus/nsSQL.o 

OBJS += \
./nsdatabaseLus/nsSQL.o 

CPP_DEPS += \
./nsdatabaseLus/nsSQL.d 


# Each subdirectory must supply rules for building sources it contributes
nsdatabaseLus/%.o: ../nsdatabaseLus/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


