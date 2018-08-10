#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include "tinysch.h"

BYTE count = 0; //a variable used to keep count of number of Blink2 function calls

void Blink0() //function to blink the PB0 LED
{
    PORTB ^= 0b1; //toggles the pin on every call
    if(count == 0xff) //if count == 0xff
    {
        PORTB &= ~(1); //turn PB0 LOW
        TinyR_RemoveTask(0); //remove the first task (this) from the list
    }
    TinyR_TaskComplete(); //task complete for the iteration
}


void Blink2() //this function will increment the count counter
{
    count++; //increment
    PORTB ^= 0b10; //toggle the LED at PB1
    TinyR_TaskComplete(); //task complete for the iteration
}

int main()
{
    cli(); //disabling interrupts
    DDRB = 0b111; //setting the first 3 PORTB pins as output
    TinyR_SimpleInit(PRESCALER_256); //setting the prescaler as 256
    TinyR_AddTask(Blink0); //adding task
    TinyR_AddTask(Blink2); //adding task
    sei(); //enabling interrupts, starting scheduler
    while(1)
    {
	}
}
