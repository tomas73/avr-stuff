#define F_CPU 16000000UL /* Clock Frequency = 16Mhz */
#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

int usart_putchar_printf(char var, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);


void usart_putchar(char data) {
	// Wait for empty transmit buffer
  while ( !(UCSRA & (_BV(UDRE))));
	// Start transmission
	UDR = data;
}

int usart_putchar_printf(char var, FILE *stream) {
	// translate \n to \r for br@y++ terminal
	if (var == '\n') usart_putchar('\r');
	usart_putchar(var);
	return 0;
}

void usart_pstr(char *s) {
	// loop through entire string

  while (*s) {
    usart_putchar(*s);
    s++;
  }
}


void usart_init( uint16_t ubrr) {
	// Set baud rate
	UBRRH = (uint8_t)(ubrr>>8);
	UBRRL = (uint8_t)ubrr;
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// Set frame format: 8data, 1stop bit
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

int main(){
  //  unsigned char control[4] = {0x01, 0x02, 0x04, 0x08};
  //  unsigned char control[8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};
  unsigned char control[8] = {0x11, 0x33, 0x22, 0x66, 0x44, 0xcc, 0x88, 0x99};

  unsigned char val = 0;
  // setup our stdio stream

  stdout = &mystdout;
  
  usart_init(MYUBRR);

  DDRB = 0xFF; // Set all the pins of PortB as output
  PORTB = 0xFF; //Turns off the output on the micro-controllers port B
  
    unsigned char value = 0xFF; // unsigned char value = 0xFF;
    
    while (1) {
      val = (val + 1) % 8;
      PORTB = control[val];
      
      //      printf("TOMAS TESTAR 0x%02x\n", control[val]);
      _delay_ms(0.9);
    }
}

