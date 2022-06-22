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


DMX_Transceiver::DMX_Transceiver() {
  
}

void DMX_Transceiver::init() {
  Serial.println("init transceiver");
  _dmx_receiver = new DMX_Receiver();
  _dmx_transmitter = new DMX_Transmitter();

  //_dmx_receiver->init();
  //_dmx_transmitter->init();

  //  TIMER 1
  //  Atmega328p (arduino nano)
  TCCR1A = 0x0000;
  //  CLK = 16MHz, we will have a bit change every 4us (250kHz) -> prescaler = 16/0,25=64
  //  set prescaler to 64 (TCCR1A_cs2=0_cs1=1_cs0=1)
  //  also set the mode to CTC (1 << 3)
  TCCR1B = (1 << 3) | (1 << 1) | 1;

  //  Set Output Compare Register A for timer 1 to 1 = 0x0001
  OCR1A = 0x0000 | 0x0001;


}

void DMX_Transceiver::start() {
  _dmx_transmitter->start();
  _dmx_receiver->start();
  //TIMSK1 = 0x0000 | (1 << 1);
}

void DMX_Transceiver::stop() {
  _dmx_transmitter->stop();
  _dmx_receiver->stop();
  TIMSK1 = TIMSK1 & ~(1 << 1);
}

void DMX_Transceiver::set_pins(dmx_digitalPin RX, dmx_digitalPin TX) {
  this->set_rx_pin(RX);
  this->set_tx_pin(TX);
}

void DMX_Transceiver::set_tx_pin(dmx_digitalPin TX) {
  _dmx_transmitter->set_pin(TX);
}

void DMX_Transceiver::set_rx_pin(dmx_digitalPin RX) {
  _dmx_receiver->set_pin(RX);
}

void DMX_Transceiver::send() {
  _dmx_transmitter->send();
}

//  
ISR(TIMER1_COMPA_vect){
  _dmx_transmitter->iterate_transmission();
}


