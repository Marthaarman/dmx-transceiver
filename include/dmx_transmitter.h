#ifndef _DMX_TRANSMITTER_H_
#define _DMX_TRANSMITTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

class DMX_Transmitter {
public:
    DMX_Transmitter();
    void init();
    void set_dmx_value(uint8_t channel, uint8_t value);
    void send();
    void interrupt();
    void set_enable_pin(uint8_t pin);
private:
    dmx_state _dmx_state = DMX_States::BREAK;
    
    uint8_t *_channel_values = (uint8_t*) calloc(513, sizeof(uint8_t));
    uint8_t *_channel_values_begin = this->_channel_values;
    uint8_t *_channel_values_end = this->_channel_values + 513;
    uint8_t _tx_enable_pin = 2;

    uint16_t _BAUDRateRegister(uint32_t desiredBAUDRate) ;
    uint16_t _byte_counter = 0;

    uint32_t _DMX_BREAK_BAUD = 100000;
    uint32_t _DMX_PAYLOAD_BAUD = 250000;
    uint32_t _DMX_BREAK_BAUD_REG;
    uint32_t _DMX_PAYLOAD_BAUD_REG;
    
    bool _flag_started = false;
    bool _flag_payload = false;
    bool _flag_send = false;
    
    void _start();
    void _stop();

    void _USART_Init_BREAK();
    void _USART_Init_DMX();
    void _USART_Write_BREAK();
    void _USART_Write_BYTE(uint8_t byte);
    void _USART_Stop();
    void _USART_TX_Interrupt();
};

#endif // _DMX_TRANSMITTER_H_