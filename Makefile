CC = avr-gcc
MMCU = atmega8
CFLAGS = -I. -I/usr/avr/include/ -g -Os -mmcu=$(MMCU)a

all: main.hex

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

main.elf: main.o
	$(CC) main.o $(CFLAGS) -o main.elf

main.o: main.c
	$(CC) main.c $(CFLAGS) -c

clean:
	rm *.hex *.elf *.o
	clear
