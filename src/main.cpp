#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"
#include "dmx.h"

uint16_t bit_counter = 0;
DMX_Transceiver *dmx_transceiver;


uint32_t prev_time;
bool value = false;

void setup() {
  Serial.begin(250000);
  
  pinMode(LED_BUILTIN, OUTPUT);
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->init();
  dmx_transceiver->set_tx_pin(DMX_DigitalPins::D2);
  dmx_transceiver->start();
  
  
  prev_time = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  dmx_transceiver->send();
}


