/*
 * atmega16_demo4_int.c
 *
 * Created: 12/15/2020 3:44:12 p. m.
 * Author : pangoro24
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int analogResult = 0;


/*Interrupt Service Routine for INT0*/
ISR(INT0_vect)
{
	PORTC|=(1<<PC3);		/* Toggle PORTC */
	_delay_ms(50);  	/* Software debouncing control delay */
	
}
ISR(INT1_vect)
{
	PORTC|=(1<<PC4);		/* Toggle PORTC */
	_delay_ms(50);  	/* Software debouncing control delay */
	
}

ISR(INT2_vect)
{
	PORTC|=(1<<PC7);		/* Toggle PORTC */
	_delay_ms(50);  	/* Software debouncing control delay */
	
}

ISR(ADC_vect) //interrupt function
{
	//The ADC generates a 10-bit result
	//which can be found in the ADC Result Registers, ADCH and ADCL
	unsigned int binary_weighted_voltage_low = ADCL; //Read 8 low bits first
	unsigned int binary_weighted_voltage_high = ((unsigned int)(ADCH << 8)); //Read 2 high bits
	analogResult = binary_weighted_voltage_low | binary_weighted_voltage_high;
}

int main(void)
{
	DDRC |= (1<<PC3)|(1<<PC4) |(1<<PC7);  	/* Set some PORT C as*/
	PORTC |= 0x00; // Set pines at LOW
	
	DDRD |= (1<<PD6);
	PORTD &= ~(1<<PD6);
	
	DDRA &= ~(1<<PA1);	//Set the Data Direction Register for the POT to input
	
	GICR |= (1<<INT1) | (1<<INT0) | (1<<INT2);		/* Enable INTS*/
	MCUCR |= (1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);  /* Trigger  */
	MCUCSR |= (1<<ISC2);
	GIFR |= (1<<INTF1) | (1<<INTF0) | (1<<INTF2); 
	
	
	ADMUX =
	(0 << REFS1) |  (1 << REFS0) |   // Sets ref. voltage to VCC
	(0 << ADLAR) |				   // 0: right adjust, 1: left adjust
	(0 << MUX4) | (0 << MUX3) | (0 << MUX2)  |	(0 << MUX1)  |	(1 << MUX0);   //  MUX bits ADC1; 0010, PA1
			
			
	ADCSRA =
	(1 << ADEN)  |     // Enable ADC
	(1 << ADSC)  |     //  Start Conversion (at setup)
	(1 << ADATE)  |     // ENABLE Auto trigger
	(0 << ADIF)  |     //
	(1 << ADIE)  |     // ENABLE ADC interrupt flag
	(1 << ADPS2) | (1 << ADPS1) |  (0 << ADPS0);  // set prescaler to 64
			
	SFIOR =
	(0<<ADTS2)|		//Free running mode bit 2
	(0<<ADTS1)|		//Free running mode bit 1
	(0<<ADTS0);		//Free running mode bit 0
	
	sei();			/* Enable Global Interrupt */
	
	while(1){
		//PORTD |= (1 << PD6);
		if(analogResult>=800){
		PORTD ^= (1 << PD6);
		_delay_ms(200);
		}
		
		}
}

