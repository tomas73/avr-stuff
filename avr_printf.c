/********************************************************************************

http://efundies.com/avr-and-printf/
Includes
********************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

/********************************************************************************
Macros and Defines
********************************************************************************/
#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1

/********************************************************************************
Function Prototypes
********************************************************************************/
void usart_init(uint16_t ubrr);
char usart_getchar( void );
void usart_putchar( char data );
void usart_pstr(char *s);
unsigned char usart_kbhit(void);
int usart_putchar_printf(char var, FILE *stream)

/********************************************************************************
Global Variables
********************************************************************************/
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

/********************************************************************************
Main
********************************************************************************/
int main( void ) {
// define some local variables
	uint8_t myvalue;

	// setup our stdio stream
	stdout = &mystdout;

	// fire up the usart
	usart_init ( MYUBRR );

	// dump some strings to the screen at power on
	myvalue = 64;
	printf("Here is myvalue as an unsigned integer: %d\n", myvalue);
	printf("Here is myvalue as a char: %c\n", myvalue);
	printf("Here is myvalue in hex: 0x%X\n", myvalue);
	printf("Here is myvalue in octal: %o\n", myvalue);

	// main loop
	while(true) {
		// do nothing
	}
}

/********************************************************************************
usart Related
********************************************************************************/
void usart_init( uint16_t ubrr) {
	// Set baud rate
	UBRRH = (uint8_t)(ubrr>>8);
	UBRRL = (uint8_t)ubrr;
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// Set frame format: 8data, 1stop bit
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void usart_putchar(char data) {
	// Wait for empty transmit buffer
	while ( !(UCSRA & (_BV(UDRE))) );
	// Start transmission
	UDR = data;
}

char usart_getchar(void) {
	// Wait for incoming data
	while ( !(UCSRA & (_BV(RXC))) );
	// Return the data
	return UDR;
}

unsigned char usart_kbhit(void) {
	//return nonzero if char waiting polled version
	unsigned char b;
	b=0;
	if(UCSRA & (1<<RXC)) b=1;
	return b;
}

void usart_pstr(char *s) {
	// loop through entire string
	while (*s) {
		usart_putchar(*s);
		s++;
	}
}

// this function is called by printf as a stream handler
int usart_putchar_printf(char var, FILE *stream) {
	// translate \n to \r for br@y++ terminal
	if (var == '\n') usart_putchar('\r');
	usart_putchar(var);
	return 0;
}
