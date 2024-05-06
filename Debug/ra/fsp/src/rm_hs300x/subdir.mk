################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/rm_hs300x/rm_hs300x.c \
../ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.c 

CREF += \
test01.cref 

C_DEPS += \
./ra/fsp/src/rm_hs300x/rm_hs300x.d \
./ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.d 

OBJS += \
./ra/fsp/src/rm_hs300x/rm_hs300x.o \
./ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.o 

MAP += \
test01.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/rm_hs300x/%.o: ../ra/fsp/src/rm_hs300x/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"D:/e2/fsp_documentation/test01/generate" -I"D:\\e2\\fsp_documentation\\test01\\src" -I"." -I"D:\\e2\\fsp_documentation\\test01\\ra\\fsp\\inc" -I"D:\\e2\\fsp_documentation\\test01\\ra\\fsp\\inc\\api" -I"D:\\e2\\fsp_documentation\\test01\\ra\\fsp\\inc\\instances" -I"D:\\e2\\fsp_documentation\\test01\\ra\\arm\\CMSIS_5\\CMSIS\\Core\\Include" -I"D:\\e2\\fsp_documentation\\test01\\ra_gen" -I"D:\\e2\\fsp_documentation\\test01\\ra_cfg\\fsp_cfg\\bsp" -I"D:\\e2\\fsp_documentation\\test01\\ra_cfg\\fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

