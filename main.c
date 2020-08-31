/*
 * main.c
 */

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* ATtiny25 */
#define SCK PB2
#define MISO PB1
#define MOSI PB0
#define SS PB3

int init_spi() {
	/* Define directions for port pins */
	DDRB |= _BV(MOSI) | _BV(SCK) | _BV(SS);
	DDRB &= ~_BV(MISO);
	_NOP();
	PORTB = _BV(MISO);

	// USICR – USI Control Register
	// USISIE – Start Condition Interrupt Enable
	// USIOIE – Counter Overflow Interrupt Enable
	// USIWM1 – Wire Mode
	// USIWM0 – Wire Mode
	// USICS1 – Clock Source Select
	// USICS0 – Clock Source Select
	// USICLK – Clock Strobe
	// USITC –  Toggle Clock Port Pin
	USICR |= _BV(USIOIE);
	USICR |= (_BV(USIWM1)*0) | _BV(USIWM0); // Three-wire mode
	USICR |= _BV(USICS1) | (_BV(USICS0)*0) | _BV(USICLK); //

	USISR |= _BV(USIOIF);


	// USISR – USI Status Register

	// USIBR – USI Buffer Register

	// USIDR – USI Data Register

//	// Enable 'Clear Timer on Compare match' and init prescaler.
//	TCCR0A = (1<<WGM01) | TC0_PS_SETTING;
//	// Init Output Compare Register.
//	OCR0A = TC0_COMPARE_VALUE;

	return 1;
}

uint8_t spi_send(uint8_t byte) {
	USIDR = byte;
	USISR = _BV(USIOIF);

	while((USISR & _BV(USIOIF)) == 0) {
		USICR |= _BV(USITC);
	}

	return USIDR;
}

int main(void) {
	init_spi();

	DDRB |= _BV(PB4);
	_NOP();

	while(1) {
		if (PORTB & _BV(PB4)) {
			PORTB &= ~_BV(PB4);
		} else {
			PORTB |= _BV(PB4);
		}

		spi_send(0xAA);
		_delay_ms(500);
	}

	return 0;
}
