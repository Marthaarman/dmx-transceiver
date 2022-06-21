#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"
#include "dmx.h"

uint16_t bit_counter = 0;

bool test = false;
DMX_Transceiver *dmx_transceiver;

void setup() {
  Serial.begin(9600);
  DDRD = 0b00001100;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  dmx_transceiver->init();
  dmx_transceiver->set_tx_pin(DMX_DigitalPins::D2);
  dmx_transceiver->start();
}

void loop() {
  // put your main code here, to run repeatedly:

  
  
}



