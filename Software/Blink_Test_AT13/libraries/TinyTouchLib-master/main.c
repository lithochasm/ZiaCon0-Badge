/*
 v0.1	06/10/2013
 
 TinyTouchLibrary Example
 
 Hardware:
	- Attiny 13A/25/45/85
	- LED on PB4
	- Touchbutton on PB3
 */ 

 
#include <avr/io.h>
#include <util/delay.h>
#include "tinytouchlib.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>


int fade = 0;
int counter = 0;



//EMPTY_INTERRUPT(TIM0_COMPA_vect)

ISR(TIM0_OVF_vect)
{

		if (fade == 1){
			if (++counter > 12) {
        	PORTB ^= _BV(PB0); // toggle LED pin
			//_delay_ms(260);
			PORTB ^= _BV(PB1); // toggle LED pin
			counter = 0;
			}
		}

}



void main(void)
{

	int button_press = 3;

	CLKPR=_BV(CLKPCE);
	CLKPR=0;			// set clock prescaler to 1 (attiny 25/45/85/24/44/84/13/13A)		

	DDRB |= _BV(PB0);		// Enable LED output pin
	DDRB |= _BV(PB1);		// Enable LED output pin
	
	tinytouch_init();

	set_sleep_mode(SLEEP_MODE_IDLE);

	digitalWrite(PB0, 0);
	digitalWrite(PB1, 0);


	while(1) {
		if (tinytouch_sense()==tt_push) {
			button_press = (button_press + 1) % 3;
		}  // Toggle LED on touch event		
		_delay_ms(10);

		//randNum = (int) analogRead(PB2);
		

		switch (button_press)               // depending on which button was pushed, we perform an action
		{
		case 1:  //ONNNNN
			{
			fade = 0;
			PORTB^=_BV(PB0);
			PORTB^=_BV(PB1);
			break;
			}

		case 2:  //BREATHE
			{
			fade = 1;
			TCCR0B |= _BV(CS02)|_BV(CS00); // set prescaler to 1024 (CLK=1200000Hz/1024/256=4Hz, 0.25s)
			TIMSK0 |= _BV(TOIE0); // enable Timer Overflow interrupt
        	sei(); // enable global interrupts
			break;
			}
		case 3: // OFF
			{
			fade = 0;
			digitalWrite(PB0, 0);
			digitalWrite(PB1, 0);
			break;
			}
		default:
			{
			fade = 0;
			digitalWrite(PB0, 0);
			digitalWrite(PB1, 0);
			break;
			}
		}



	} // end while	


				
}
