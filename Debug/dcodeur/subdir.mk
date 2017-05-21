################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../dcodeur/nsdkd.cpp \
../dcodeur/nsgen.cpp \
../dcodeur/nsgenEn.cpp \
../dcodeur/nsgenFr.cpp \
../dcodeur/nsphrase.cpp 

OBJS += \
./dcodeur/nsdkd.o \
./dcodeur/nsgen.o \
./dcodeur/nsgenEn.o \
./dcodeur/nsgenFr.o \
./dcodeur/nsphrase.o 

CPP_DEPS += \
./dcodeur/nsdkd.d \
./dcodeur/nsgen.d \
./dcodeur/nsgenEn.d \
./dcodeur/nsgenFr.d \
./dcodeur/nsphrase.d 


# Each subdirectory must supply rules for building sources it contributes
dcodeur/%.o: ../dcodeur/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


