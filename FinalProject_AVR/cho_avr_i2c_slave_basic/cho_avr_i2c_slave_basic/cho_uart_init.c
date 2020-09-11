// S/W Environment : AVR Studio + WINAVR Compiler
// Target : M128
// Crystal: 16Mhz
//
// Made by chowk.
// UART0 Init

#include <avr/io.h>
#include <avr/interrupt.h>

#include "cho_uart_init.h"
#include "cho_circular_buffer.h"

#define FOSC 16000000    // Clock Speed
#define BAUD 19200
#define MYUBRR FOSC/16/BAUD-1

// UART initialize
// baud rate: 19200
// char size: 8 bit
// parity: Disabled
void uart0_init()
{
	unsigned int ubrr;

	ubrr = MYUBRR;
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void uart_init()
{
	uart0_init();
	//RX0 Complete Interrupt Enable
	UCSR0B |= (1 << RXCIE0);
}

unsigned char rx0_char(void)
{
	// Wait for data to be received
	while ( !(UCSR0A & (1 << RXC0)) );
	// Get and return received data from buffer
	return UDR0;
}

// UART0 �� �̿��� ���
void tx0_char(unsigned char data)
{
	// Wait for empty transmit buffer
	while ( !(UCSR0A & (1 << UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

// USART0, Rx Interrupt ó�� Routine
ISR(USART0_RX_vect)
{
	char c;

	// �Էµ� ���ڸ� ó�� �ϴ� ���� ���ο� Interrupt�� �߻� ���� �ʵ���
	// Rx Interrupt�� Disable �Ѵ�.
	UCSR0B &= ~(1 << RXCIE0);	// Rx Complete Interrupt disable

	c = UDR0;
	// �Էµ� ���ڰ� CR or BS Code �� �ƴ� ���
	if(!((c == CR) | (c == BS))){
		// Buffer�� ���ڸ� ���� �Ѵ�.
		putchar2buffer(c);
		// Buffer�� ���ڸ� ���� �ϴ� ���� ������ ��� ���ڸ� ����Ϳ� ��� �Ѵ�.
		// buffer_full_flag �� TRUE �� ���� ���� Buffer �� Full �����̱� ������
		// ���ڸ� Buffer�� ���� ���� ���� ��� �̴�.
		if(buffer_full_flag == FALSE)tx0_char(c);
	}
	// ���� �Էµ� ���ڰ� BS Code�� ��� Buffer���� ���� �ϳ��� ��� �Ѵ�.
	if(c == BS){
		backspace2buffer();
		tx0_char(BS);
	}

	// ���ο� ���ڸ� ���� �� �ֵ��� Rx Interrupt�� Enable �Ѵ�.
	UCSR0B |= (1 << RXCIE0); // Rx Complete Interrupt enable

	// ���� �Էµ� ���ڰ� CR Code�� ��� Tx Interrupt�� Enable �Ѵ�.
	// CTx Interrupt�� Enable �Ǹ� Tx Interrupt ó�� Routine ����
	// Buffer�� ������ ����Ϳ� ��� �ϰ� �ȴ�.
	if(c == CR){
		UCSR0B |= (1 << TXCIE0); // Tx Complete Interrupt enable
		tx0_char(CR); tx0_char(LF);
	}
} 

// USART0, Tx Interrupt ó�� Routine
ISR(USART0_TX_vect)
{
	char c;

	// Tx Interrupt�� ó�� �ϴ� ���� ���ο� Rx Interrupt�� �߻� ���� �ʵ���
	// Rx Interrupt�� Disable �Ѵ�.
	UCSR0B &= ~(1 << RXCIE0);	// Rx Complete Interrupt disable

	// Interrupt ó���� �ϴ� ���� ���ο� Interrupt�� �߻� ���� �ʵ���
	// Tx Interrupt�� Disable �Ѵ�.
	UCSR0B &= ~(1 << TXCIE0);	// Tx Complete Interrupt disable

	// Buffer�� ���� �ϳ��� ���ڸ� ���� �´�.
	c = getchar_buffer();
	// ���� ���ڰ� �ҹ����̸� �빮�ڷ� ��ȯ �Ѵ�.
	if((c >= 'a') && (c <= 'z')) c &= 0b11011111;
	// Buffer�� ���� ���������� ���ڸ� ������ ���  
	if(c != 0){
		// ����Ϳ� ���ڸ� ��� �Ѵ�.
		tx0_char(c);
		// ���ο� ���ڸ� ��� �� �� �ֵ��� Tx Interrupt�� Ensable �Ѵ�.
		UCSR0B |= (1 << TXCIE0); // Tx Complete Interrupt enable
    }
	// ���ο� ���ڸ� ���� �� �ֵ��� Rx Interrupt�� Ensable �Ѵ�.
	else {
		tx0_char(CR); tx0_char(LF);
		UCSR0B |= (1 << RXCIE0); // Rx Complete Interrupt enable
	}
}
