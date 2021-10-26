/*
 * main.c
 *
 * Created: 10/26/2021 9:16:02 AM
 *  Author: ccons
 */ 

#define F_CPU 8000000UL

#include <xc.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/bit_operators.h"
#include "HAL/UART1/UART1_HAL.h"

void send(char str[]);
void setPin(char msg[], char port, uint8_t pin, char state);


void script(){
	char strIn[100];
	char strOut[200];
	
	//Wait and read msg
	while(!uart1_hal_message_ready());
	uart1_hal_read_message_as_str(strIn);
	
	if(strlen(strIn)!=5){
		sprintf(strOut, "Command too short or long, must be five");
		send(strOut);
		return;
	}
	
	uint8_t pin=atoi(&strIn[2]);
	if (pin<0 || pin>7)
	{
		sprintf(strOut, "Fail, not valid pin number");
		send(strOut);
		return;
	}
	
	if (!(strIn[4]=='0' || strIn[4]=='1'))
	{
		sprintf(strOut, "Fail, use 0 for low 1 for high");
		send(strOut);
		return;
	}
	
	sprintf(strOut, "%s success", (const char *) strIn);
	setPin(strOut, strIn[1], pin, strIn[4]);
	send(strOut);
	
	_delay_ms(1000);
}

void setPin(char msg[], char port, uint8_t pin, char state){
	
	switch(port){
		case 'A':
			if(state=='1'){
				set_bit(PORTA, pin);
			}else{
				clear_bit(PORTA, pin);
			}
		break;
		
		case 'B':
			if(state=='1'){
				set_bit(PORTB, pin);
			}else{
				clear_bit(PORTB, pin);
			}
		break;
		
		case 'C':
			if(state=='1'){
				set_bit(PORTC, pin);
				}else{
				clear_bit(PORTC, pin);
			}
		break;
		
		case 'D':
			if(state=='1'){
				set_bit(PORTD, pin);
			}else{
				clear_bit(PORTD, pin);
			}
		break;
		
		default:
			sprintf(msg, "Unknown port");
	}
	
}

void send(char str[]){
	strncat(str, "\r\n", 2);
	uart1_hal_send_message((uint8_t *) str, strlen(str));
}


void init_pins(){
	set_byte(DDRA, 0xff);
	set_byte(DDRB, 0xff);
	set_byte(DDRC, 0xff);
	set_byte(DDRD, 0xff);
}




int main(void)
{
	init_pins();
	uart1_hal_init();
	char str[]="Ready";
	send(str);
	
	
    while(1)
    {
		script();
		
    }
}