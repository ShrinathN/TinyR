#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "tinysch.h"

void blinky()
{
    PORTB ^= 0b1;
}

void blinky2()
{
    PORTB ^= 0b10;
}

int main()
{
    DDRB |= 0b11;
    TinyR_SimpleInit(PRESCALER_1024);
    TinyR_AddTask(blinky);
    TinyR_AddTask(blinky2);
	while(1)
	{
	}
}
