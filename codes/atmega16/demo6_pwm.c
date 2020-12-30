/*
 * atmega16_demo6_pwm.c
 *
 * Created: 12/21/2020 12:10:17 p. m.
 * Author : pangoro24
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    TCCR0 = (1<<WGM01) | (1<<WGM00) | //Set as FAST PWM
			(1<<COM01) | (0<<COM00) | // Set as non-inverted output, 
			(0<<CS02)  | (0<<CS01)  | (1<<CS00);  
    DDRB|=(1<<PB3); //Set OC0 (dedicated pwm pin at board) as output

	TCCR1A |= (1 << COM1B1) | (0 << COM1B0) // configured as no-inverting mode
			| (0 << WGM11)  | (1 << WGM10); // Waveform generated as
	TCCR1B |= (0<<WGM13) | (1<<WGM12) |     //  Fast pwm 8 bit
			  (0 << CS12)| (1 << CS11)| (0 << CS10); // set pre-scaler to 8 
		
	OCR1B=64;//set at 25% pwm duty cycle ((64+1)/256 = 
	
	DDRD|=(1<<PD4); //Set OC1B
	 
	while (1)
	{
		unsigned char duty;
		
		for(duty=0; duty<255; duty++)
		{
			OCR0=duty;  //from low to high intensity
			_delay_ms(250);
		}
		for(duty=255; duty>1; duty--)
		{
			OCR0=duty;  /*decrease the LED light intensity*/
			_delay_ms(250);
		}
	}
}

