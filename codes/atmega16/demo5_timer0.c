/*
 * atmega16_demo5_timer0.c
 *
 * Created: 12/20/2020 9:26:03 p. m.
 * Author : pangoro24
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void delay_desired();

ISR(TIMER0_OVF_vect)
{
	PORTC ^= (1 << PC4);		// Toggle led 
	TCNT0 = 0x9c;
}

int main(void)
{
	//cli();
	DDRC |= (1<<PC4);		/* PORTB as output */
	PORTC |= 0x00;
	
	//TIMSK=(1<<TOIE0);  	/* Enable Timer0 overflow interrupt */
	//sei();
	while(1)  		/* Repeat forever */
	{
		
		//USAGE OF TIMER WITHOUT INTERRUPT
		
		PORTC ^= (1 << PC4);
		//_delay_ms(50);
		delay_desired();

	}
}

void delay_desired()
{
	
	//TCCR0 |= (1<<CS02) | (0<<CS01) | (1<<CS00); /* Timer mode: NORMAL /pre-scaler 1024 */
	//TCNT0 = 0x9c;  		// Limiter for 100 ms using 1024 pr
	
	TCCR0 |= (1<<CS02) | (0<<CS01) | (0<<CS00); //pre-scaler 256 (1 cycle = 256ns)
	TCNT0 = 0x3d; //required cycles are 195 counting up from 61 till 255 for 50ms
	
	while((TIFR&0x01)==0);  // Wait till overflow 
	TCCR0 = 0;
	TIFR = 0x1;   //Clear TOV0 timer overflow flag
	
}

