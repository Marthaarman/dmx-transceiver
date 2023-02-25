#ifndef _DMX_TRANSMITTER_H_
#define _DMX_TRANSMITTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

class DMX_Transmitter {
public:
    //  constructor
    DMX_Transmitter();

    //  set the pin that is enabled during transmission
    //  this pin is set to high when enabled
    //  used for devices like the max485
    //  default pin 2
    void set_enable_pin(uint8_t pin);

    //  initializes the transmitter
    //  calculates the baud rates for payload and break states
    void init();

    //  stores a user dmx input to a given channel
    //  channels 1-512
    //  values 0-255
    void set_dmx_value(uint16_t channel, uint8_t value);

    //  transmits the actual data using the DMX protocol
    //  only triggers once, rest is done using interrupts
    void transmit();

    //  interrupts are triggered each time a (previous) data byte has been transmitted
    //  first byte initiated by the transmit() function
    void interrupt();
    
private:
    
    //  pointers for channel data
    //  memory for 513 bytes are reserved
    //  byte 0 is the byte used for the start code
    //  begin and end pointers are used
    uint8_t *_channel_values = (uint8_t*) calloc(513, sizeof(uint8_t));
    uint8_t *_channel_values_begin = this->_channel_values;
    uint8_t *_channel_values_end = this->_channel_values + 513;

    //  default pin that is enabled during transmission
    uint8_t _tx_enable_pin = 2;

    //  counter used during transmission to keep track on which byte to transmit
    uint16_t _byte_counter = 0;

    //  variables used for baud rate registers
    uint32_t _DMX_BREAK_BAUD = 100000;
    uint32_t _DMX_PAYLOAD_BAUD = 250000;
    uint32_t _DMX_BREAK_BAUD_REG;
    uint32_t _DMX_PAYLOAD_BAUD_REG;
    
    //  flag booleans
    bool _flag_started = false;     //  when transmission started, set to high
    bool _flag_payload = false;     //  when break has been transmitted, allow the payload to be transmitted
    bool _flag_send = false;        //  when the send (transmit) function is called
    
    //  start and stop functions
    //  used to set flags
    void _start();
    void _stop();

    //  USART functions
    //  functions that write to the registers
    void _USART_Init_BREAK();               //  set the baud rate to break speed (baud 100000), enable the interrupts
    void _USART_Init_DMX();                 //  set the baud rate to payload speed (baud 250000), enable interrupts
    void _USART_Write_BREAK();              //  write zeros using break speed
    void _USART_Write_BYTE(uint8_t byte);   //  write start code and 512 bytes of payload using payload speed
    void _USART_Stop();                     //  disable USART and interrupts
    void _USART_TX_Interrupt();             //  the function that is called when USART interrupt is triggered

    //  function to calculate the baud rate for the UART register
    uint16_t _BAUDRateRegister(uint32_t desiredBAUDRate) ;
};

#endif // _DMX_TRANSMITTER_H_