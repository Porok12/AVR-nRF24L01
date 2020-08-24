/*
 * main.c
 */

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int main(void) {
	/* Define directions for port pins */
	DDRB |= 1 << PB4;
	_NOP();

	while(1) {
		PORTB &= ~(1 << PB4);
		_delay_ms(500);
		PORTB |= 1 << PB4;
		_delay_ms(500);
	}

	return 0;
}
