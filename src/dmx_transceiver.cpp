#include "dmx_transceiver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "dmx_transmitter.h"
#include "dmx_receiver.h"
#include "dmx_boards.h"

DMX_Transmitter *_dmx_transmitter;
DMX_Receiver    *_dmx_receiver;

DMX_Transceiver::DMX_Transceiver() {}

void DMX_Transceiver::init() {
  _dmx_transmitter = new DMX_Transmitter();
  _dmx_receiver = new DMX_Receiver();
}

void DMX_Transceiver::set_tx_enable_pin(uint8_t pin) {
  _dmx_transmitter->set_enable_pin(pin);
}

void DMX_Transceiver::set_rx_enable_pin(uint8_t pin) {
  
  _dmx_receiver->set_enable_pin(pin);
}

void DMX_Transceiver::transmit() {
  _dmx_transmitter->transmit();
}

void DMX_Transceiver::receive() {
  _dmx_receiver->receive();
}

void DMX_Transceiver::set_dmx_value(uint16_t channel, uint8_t value) {
  //  channel 0 should not be accessed
  //  channels match real 1-512 channels
  //  values must be 255 or smaller and at least 0
  if(channel > 0 && channel < 513 && value >= 0 && value < 256) {
    _dmx_transmitter->set_dmx_value(channel, value);
  }
}

uint8_t DMX_Transceiver::get_dmx_value(uint16_t channel) {
  //  channel 0 should not be accessed
  //  channels match real 1-512 channels
  if(channel > 0 && channel < 513) {
    return _dmx_receiver->get_dmx_value(channel);
  }
  return 0;
}

//  interrupt call for the transmitter
//  called when the actual byte was sent.
ISR(USART_TX_vect) {
  _dmx_transmitter->interrupt();
}

//  interrupt call for the transmitter
//  called after data register was emptied by handing it over to the shift register.
ISR(USART_UDRE_vect) {
  _dmx_transmitter->interrupt();
}

//  interrupt for the receiver
//  called when a byte has been received
ISR(USART_RX_vect) {
  _dmx_receiver->interrupt();
}