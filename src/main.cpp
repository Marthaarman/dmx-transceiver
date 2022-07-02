#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"

// transceiver instance best to be pointer
DMX_Transceiver *dmx_transceiver;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //  initialize a new transceiver instane
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->init();  
}


void loop() {
  // put your main code here, to run repeatedly:
  
  dmx_transceiver->set_dmx_value(1, 255);
  dmx_transceiver->set_dmx_value(2, 255);
  dmx_transceiver->set_dmx_value(3, 255);
  dmx_transceiver->set_dmx_value(4, 255);
  dmx_transceiver->set_dmx_value(5, 255);
  dmx_transceiver->transmit();
  delay(50);
  
  
  // dmx_transceiver->receive();
  
}



