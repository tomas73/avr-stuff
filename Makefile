### Connect atmega32 "ISP" to STK500v2 "ISP6PIN"

CC=avr-gcc

OBJCOPY=avr-objcopy

CFLAGS=-g -mmcu=atmega32

#Info til AVRDUDE
PROGRAMMER = avrdude
BOARD = stk500v2
MCU = atmega32
PORT = /dev/ttyUSB0


PFLAGS = -p $(MCU) -c $(BOARD) -P $(PORT)

rom.hex : bitmanipulation.out
	$(OBJCOPY) -j .text -j .data -O ihex bitmanipulation.out rom.hex

bitmanipulation.out : bitmanipulation.o
	$(CC) $(CFLAGS) -o bitmanipulation.out -Wl,-Map,bitmanipulation.map bitmanipulation.o

bitmanipulation.o : bitmanipulation.c
	$(CC) $(CFLAGS) -Os -c bitmanipulation.c

clean:
	rm -f *.o *.out *.map *.hex

program:
	$(PROGRAMMER) $(PFLAGS) -F -e
	$(PROGRAMMER) $(PFLAGS) -U flash:w:rom.hex

