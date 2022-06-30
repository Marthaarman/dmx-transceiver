#include "dmx_transceiver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "dmx.h"
#include "dmx_transmitter.h"
#include "dmx_receiver.h"

DMX_Transmitter *_dmx_transmitter;
DMX_Receiver    *_dmx_receiver;

bool LO = 0;
bool HI = 1;


DMX_Transceiver::DMX_Transceiver() {}

void DMX_Transceiver::init() {
  _dmx_transmitter = new DMX_Transmitter();
  _dmx_receiver = new DMX_Receiver();
}

void DMX_Transceiver::start() {}

void DMX_Transceiver::stop() {}

void DMX_Transceiver::set_tx_enable_pin(uint8_t pin) {
  _dmx_transmitter->set_enable_pin(pin);
}

void DMX_Transceiver::transmit() {
  _dmx_transmitter->transmit();
}

void DMX_Transceiver::receive() {
  _dmx_receiver->receive();
}

void DMX_Transceiver::set_dmx_value(uint8_t channel, uint8_t value) {
  if(channel > 0 && channel < 513 && value >= 0 && value < 256) {
    _dmx_transmitter->set_dmx_value(channel, value);
  }
}

uint8_t DMX_Transceiver::get_dmx_value(uint8_t channel) {
  if(channel > 0 && channel < 513) {
    return _dmx_receiver->get_dmx_value(channel);
  }
  return 0;
}

// Interrupt service routines that are called when the actual byte was sent.
ISR(USART_TX_vect)
{
  _dmx_transmitter->interrupt();
} // ISR(USART_TX_vect)


// this interrupt occurs after data register was emptied by handing it over to the shift register.
ISR(USART_UDRE_vect)
{
  _dmx_transmitter->interrupt();
} // ISR(USART_UDRE_vect)


ISR(USART_RX_vect) {
  _dmx_receiver->interrupt();
} //  ISR(USART_RX_vect)