#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"

//  the pins that will be set HIGH when in that mode (for max485 module)
#define RX_ENABLE_PIN 6
#define TX_ENABLE_PIN 5

// transceiver instance best to be pointer
DMX_Transceiver *dmx_transceiver;


//  listener channel
uint16_t listener_channel = 511;

void setup() {
  //  initialize a new transceiver instance
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->set_rx_enable_pin(RX_ENABLE_PIN);
  dmx_transceiver->set_tx_enable_pin(TX_ENABLE_PIN);
  dmx_transceiver->init();  
}


//  forward all incoming to out
void set_output_dmx() {
  for(int i = 1; i <= 512; i++) {
      dmx_transceiver->set_dmx_value(i, dmx_transceiver->get_dmx_value(i));
    }
}

//  if channel "listener_channel" >= 250, channel 1 will be HIGH
//  if channel "listener_channel" < 250, channel 1 will be LOW
void alter_output_dmx() {
  if(dmx_transceiver->get_dmx_value(listener_channel) >= 250) {
    dmx_transceiver->set_dmx_value(1, 255);
  }else {
    dmx_transceiver->set_dmx_value(1, 0);
  }
}


void loop() {
  // put your main code here, to run repeatedly:

  //  listen for incoming dmx
  //  returns when a packet is received or after a timeout
  dmx_transceiver->receive();


  //  forward all incoming dmx packets to the output
  set_output_dmx();

  //  alter the output
  alter_output_dmx();
  
  //  transmit the output      
  dmx_transceiver->transmit();
}
