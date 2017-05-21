################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dtilib/sockets/sock.cpp \
../dtilib/sockets/sockstream.cpp 

OBJS += \
./dtilib/sockets/sock.o \
./dtilib/sockets/sockstream.o 

CPP_DEPS += \
./dtilib/sockets/sock.d \
./dtilib/sockets/sockstream.d 


# Each subdirectory must supply rules for building sources it contributes
dtilib/sockets/%.o: ../dtilib/sockets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


