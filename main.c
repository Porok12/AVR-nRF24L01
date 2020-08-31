/*
 * main.c
 */

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "nRF24L01.h"

/* ATtiny25 */
#define SCK PB2
#define MISO PB1
#define MOSI PB0
#define SS PB3
#define CSN SS

int init_spi() {
	/* Define directions for port pins */
	DDRB |= _BV(MISO) | _BV(SCK) | _BV(SS);
	DDRB &= ~_BV(MOSI);
	_NOP();
	PORTB = _BV(MOSI);

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

	_NOP();
	PORTB |= _BV(CSN);
//	PORTB |= _BV(CE);

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

uint8_t get_reg(uint8_t reg) {
	_delay_us(10);
	PORTB &= ~_BV(CSN);
	_delay_us(10);
	spi_send(R_REGISTER + reg);
	_delay_us(10);
	reg = spi_send(NOP);
	_delay_us(10);
	PORTB |= _BV(CSN);
	return reg;
}

void set_reg(uint8_t reg, uint8_t package) {
	_delay_us(10);
	PORTB &= ~_BV(CSN);
	_delay_us(10);
	spi_send(W_REGISTER + reg);
	_delay_us(10);
	spi_send(package);
	_delay_us(10);
	PORTB |= _BV(CSN);
}


int main(void) {
	init_spi();

	DDRB |= _BV(PB4);
	_NOP();

	while(1) {
		if (get_reg(STATUS) == 0x0E) {
			PORTB &= ~_BV(PB4);
		} else {
			PORTB |= _BV(PB4);
		}

		_delay_ms(500);
	}

	return 0;
}
