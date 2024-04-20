/*
 * main.c
 *
 *  Created on: 23 lis 2023
 *      Author: marce
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "lcd_displ.h"


#define RGB_RED_PIN (1<<PD0)
#define RGB_RED_OFF PORTD &= ~RGB_RED_PIN
#define RGB_RED_ON PORTD |= RGB_RED_PIN
#define RGB_RED_TOG PORTD ^= RGB_RED_PIN

#define RGB_GREEN_PIN (1<<PD6)
#define RGB_GREEN_OFF PORTD &= ~RGB_GREEN_PIN
#define RGB_GREEN_ON PORTD |= RGB_GREEN_PIN
#define RGB_GREEN_TOG PORTD ^= RGB_GREEN_PIN

#define KEY_PIN (1<<PB1)
#define KEY_DOWN !(PINB & KEY_PIN)

/*#define MOTOR_PIN (1<<PD7)
#define MOTOR_ON PORTD |= MOTOR_PIN
#define MOTOR_OFF PORTD &= ~MOTOR_PIN
#define MOTOR_TOG PORTD ^= MOTOR_PIN
*/


// funkcja do uczerwienienia diody rgb
uint8_t czerwonosc(void);

int main(void) {

	DDRD |= (RGB_RED_PIN | RGB_GREEN_PIN);
	PORTD |= RGB_RED_PIN; //œwieci na czerwono *stop* w czasie 0
	PORTD &= ~(RGB_GREEN_PIN); //zielony wy³¹czony w czasie 0
	DDRB &= ~KEY_PIN;
	PORTB |= KEY_PIN;

	// Ustawienie trybu szybkiego PWM dla Timer0
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);


	lcd_init();
	lcd_clear();
	while (1)
	{

		lcd_gotoxy(0,0);

    	OCR0A = czerwonosc();
    	if(czerwonosc()==255)
    	{

    		lcd_printf("     JAZDA     ");
    	}
    	else
    	{
    		lcd_printf("DO STARTU: %d   ",6-OCR0A/10);
    	}

    }

    	return 0;
    }

uint8_t czerwonosc(void)
{
	static uint8_t mocPWM=0;
	if (KEY_DOWN)
	{
		_delay_ms(80);
		if (KEY_DOWN)
		{
			if(mocPWM < 50)
			{
				TCCR0B |= (1 << CS01);
				mocPWM += 10;
			}
			else if(mocPWM >= 50 && mocPWM<255)
			{
				mocPWM = 255;
				RGB_RED_OFF;

			}
			else
			{
				RGB_RED_ON;
				TCCR0B &= ~(1 << CS01);
				mocPWM = 0;
				TCCR0B &= ~(1 << CS01);

			}

			_delay_ms(200);

		}
	}
	return mocPWM;

}

