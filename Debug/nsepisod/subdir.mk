################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nsepisod/eptables.cpp \
../nsepisod/flechiesDB.cpp \
../nsepisod/nsclasser.cpp \
../nsepisod/nsldvuti.cpp 

OBJS += \
./nsepisod/eptables.o \
./nsepisod/flechiesDB.o \
./nsepisod/nsclasser.o \
./nsepisod/nsldvuti.o 

CPP_DEPS += \
./nsepisod/eptables.d \
./nsepisod/flechiesDB.d \
./nsepisod/nsclasser.d \
./nsepisod/nsldvuti.d 


# Each subdirectory must supply rules for building sources it contributes
nsepisod/%.o: ../nsepisod/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


