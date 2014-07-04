CC = arm-none-eabi-gcc
DEVICE_INCLUDES = -I ./Libraries/CMSIS/Device/ST/STM32F0xx/Include
ARM_INCLUDES = -I ./Libraries/CMSIS/Include
CFLAGS = ${DEVICE_INCLUDES} ${ARM_INCLUDES} -mcpu=cortex-m0 -mthumb -g -std=c99

AS = arm-none-eabi-as
ASFLAGS = -mcpu=cortex-m0 -mthumb -g

LD = arm-none-eabi-ld
LINKER_FILE = stm32f0_linker.ld
LDFLAGS = -T $(LINKER_FILE) -nostartfiles

OBJS = \
       main.o \
       startup_stm32f051.o \
       system_stm32f0xx.o \
       serial_terminal.o

main.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.elf $(OBJS)
	
main.o: main.c serial_terminal.c serial_terminal.h
serial_terminal.o: serial_terminal.c serial_terminal.h
system_stm32f0xx.o: system_stm32f0xx.c
startup_stm32f051.o: startup_stm32f051.s

clean:
	$(RM) *.o *.elf *.bin
