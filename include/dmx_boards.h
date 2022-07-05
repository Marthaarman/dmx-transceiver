
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef _DMX_BOARDS_H_
#define _DMX_BOARDS_H_

#define atmega328p

//  SOURCE:
//  https://github.com/mathertel/DMXSerial/blob/master/src/DMXSerial_avr.h

//  Arduino micro
//  atmega32u4
#ifdef atmega32u4

#define DMX_UCSRA UCSR1A // Control and Status Register A
#define DMX_UCSRB UCSR1B // USART Control and Status Register B

#define DMX_RXCIE RXCIE1 // Enable Receive Complete Interrupt
#define DMX_TXCIE TXCIE1 // Enable Transmission Complete Interrupt
#define DMX_RXEN RXEN1 // Enable Receiving
#define DMX_TXEN TXEN1 // Enable Sending

#define DMX_UCSRC UCSR1C // Control and Status Register C

#define DMX_UBRRH UBRR1H // USART Baud Rate Register High
#define DMX_UBRRL UBRR1L // USART Baud Rate Register Low

#define DMX_UDR UDR1 // USART Data Register
#define DMX_FE FE1 // Frame Error

#define DMX_USART_RX_vect USART_RXC_vect // Interrupt Data received
#define DMX_USART_TX_vect USART_TXC_vect // Interrupt Data sent
#define DMX_USART_UDRE_vect USART_UDRE_vect // Interrupt Data Register empty
#endif //atmega32u4

//  Arduino uno / Arduino nano
//  atmega328p
#ifdef atmega328p
#define DMX_UCSRA UCSR0A // Control and Status Register A
#define DMX_UCSRB UCSR0B // USART Control and Status Register B

#define DMX_RXCIE RXCIE0 // Enable Receive Complete Interrupt
#define DMX_TXCIE TXCIE0 // Enable Transmission Complete Interrupt
#define DMX_RXEN RXEN0 // Enable Receiving
#define DMX_TXEN TXEN0 // Enable Sending

#define DMX_UCSRC UCSR0C // Control and Status Register C

#define DMX_UBRRH UBRR0H // USART Baud Rate Register High
#define DMX_UBRRL UBRR0L // USART Baud Rate Register Low

#define DMX_UDR UDR0 // USART Data Register
#define DMX_FE FE0 // Frame Error

#define DMX_USART_RX_vect USART_RXC_vect // Interrupt Data received
#define DMX_USART_TX_vect USART_TXC_vect // Interrupt Data sent
#define DMX_USART_UDRE_vect USART_UDRE_vect // Interrupt Data Register empty

#endif //   atmega328p


#endif