/*
 * atmega16_demo3_adc.c
 *
 * Created: 12/10/2020 4:02:59 p. m.
 * Author : pangoro24
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int analogResult = 0;
unsigned int binary_weighted_voltage_low;
unsigned int binary_weighted_voltage_high;


int main(void)
{
    	DDRC |= (1<<PC3)|(1<<PC4);   //Set the Data Direction Register to output (PB3 = yellow led , PB6 = green led)
    	DDRA &= ~(1<<PA1);	//Set the Data Direction Register for the POT to input
    
		ADMUX =
		(0 << REFS1) |  (1 << REFS0) |   // Sets ref. voltage to VCC
		(0 << ADLAR) |				   // 0: right adjust, 1: left adjust
		(0 << MUX4) | (0 << MUX3) | (0 << MUX2)  |	(0 << MUX1)  |	(1 << MUX0);   //  MUX bits ADC1; 0010, PA1
		
		
		ADCSRA =
		(1 << ADEN)  |     // Enable ADC
		(0 << ADSC)  |     // DISABLED Start Conversion (at setup)
		(0 << ADATE)  |     // DISABLED Auto trigger
		(0 << ADIF)  |     //
		(0 << ADIE)  |     // DISABLED ADC interrupt flag
		(1 << ADPS2) | (1 << ADPS1) |  (1 << ADPS0);  // set prescaler to 128
		
		SFIOR =
		(0<<ADTS2)|		//Free running mode bit 2
		(0<<ADTS1)|		//Free running mode bit 1
		(0<<ADTS0);		//Free running mode bit 0
	
 
 while (1)
 {
	 ADCSRA |= (1 << ADSC);         // start ADC measurement
	 while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
	 //result = (ADCH<<8)|ADCL; 8 bits from ADCL + 2 bits from ADCH
	 binary_weighted_voltage_low = ADCL;
	 binary_weighted_voltage_high = ((unsigned int)(ADCH << 8));
	 analogResult = binary_weighted_voltage_low | binary_weighted_voltage_high;
	 //VCC = 4.8V - 1 unit = 4.6875 mV
	 if(analogResult<=200) //value 0-200 (0-0.96V @vref=4.93 V)
	 {
		 PORTC|=(1<<PC3); // turn yellow led on
		 PORTC&=~(1<<PC4); // turn green led off
		 _delay_ms(100);
	 }
	 else if(analogResult>=900) //value 900-1023 (4.33 - 4.93 V @vref = 4.93 V)
	 {
		 PORTC&=~(1<<PC3);
		 PORTC|=(1<<PC4);
		 _delay_ms(100);
	 }
	 else
	 {
		 PORTC &=~((1<<PC4)|(1<<PC3));
	 }
 }
 }

