################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../enterpriseLus/personLus.cpp \
../enterpriseLus/requestEngineLus.cpp \
../enterpriseLus/superLus.cpp 

O_SRCS += \
../enterpriseLus/personLus.o 

OBJS += \
./enterpriseLus/personLus.o \
./enterpriseLus/requestEngineLus.o \
./enterpriseLus/superLus.o 

CPP_DEPS += \
./enterpriseLus/personLus.d \
./enterpriseLus/requestEngineLus.d \
./enterpriseLus/superLus.d 


# Each subdirectory must supply rules for building sources it contributes
enterpriseLus/%.o: ../enterpriseLus/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


