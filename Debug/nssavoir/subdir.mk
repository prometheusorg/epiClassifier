################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nssavoir/Nsfilgd.cpp \
../nssavoir/nsconver.cpp \
../nssavoir/nsguide.cpp \
../nssavoir/nslocalise.cpp \
../nssavoir/nspatbas.cpp \
../nssavoir/nspatho.cpp \
../nssavoir/nsvarray.cpp 

OBJS += \
./nssavoir/Nsfilgd.o \
./nssavoir/nsconver.o \
./nssavoir/nsguide.o \
./nssavoir/nslocalise.o \
./nssavoir/nspatbas.o \
./nssavoir/nspatho.o \
./nssavoir/nsvarray.o 

CPP_DEPS += \
./nssavoir/Nsfilgd.d \
./nssavoir/nsconver.d \
./nssavoir/nsguide.d \
./nssavoir/nslocalise.d \
./nssavoir/nspatbas.d \
./nssavoir/nspatho.d \
./nssavoir/nsvarray.d 


# Each subdirectory must supply rules for building sources it contributes
nssavoir/%.o: ../nssavoir/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


