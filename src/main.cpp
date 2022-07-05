#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"

#define SWITCH_PIN 9

// transceiver instance best to be pointer
DMX_Transceiver *dmx_transceiver;

typedef enum TRANSCEIVER_States {
  SLAVE_MODE,
  MASTER_MODE
};

typedef enum SWITCH_States {
  ON,
  OFF,
  UNKNOWN 
};


TRANSCEIVER_States current_tranceiver_state = MASTER_MODE;
TRANSCEIVER_States next_tranceiver_state = current_tranceiver_state;
SWITCH_States current_switch_state = UNKNOWN;
SWITCH_States prev_switch_state = current_switch_state;

bool current_switch_value;
bool prev_switch_value;




void setup() {
  pinMode(SWITCH_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //  initialize a new transceiver instane
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->init();  

  if(digitalRead(SWITCH_PIN) == 1) {
    current_switch_value = true;
    prev_switch_value = true;
  }else {
    current_switch_value = false;
    prev_switch_value = false;
  }
}


void detect_transceiver_mode() {
  if(dmx_transceiver->get_dmx_value(512) >= 250) {
    current_tranceiver_state = TRANSCEIVER_States::SLAVE_MODE;
  }else {
    current_tranceiver_state = TRANSCEIVER_States::MASTER_MODE;
  }
}

void switch_light() {
  if(current_switch_state == SWITCH_States::ON) {
    digitalWrite(LED_BUILTIN, HIGH);
    dmx_transceiver->set_dmx_value(1, 255);
    dmx_transceiver->set_dmx_value(2, 255);
    dmx_transceiver->set_dmx_value(3, 255);
    dmx_transceiver->set_dmx_value(4, 255);
    dmx_transceiver->set_dmx_value(5, 255);
  }else if(current_switch_state == SWITCH_States::OFF) {
    digitalWrite(LED_BUILTIN, LOW);
    dmx_transceiver->set_dmx_value(1, 0);
    dmx_transceiver->set_dmx_value(2, 0);
    dmx_transceiver->set_dmx_value(3, 0);
    dmx_transceiver->set_dmx_value(4, 0);
    dmx_transceiver->set_dmx_value(5, 0);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
  dmx_transceiver->receive();
  detect_transceiver_mode();
  current_switch_value = digitalRead(SWITCH_PIN);
  
  switch(current_tranceiver_state) {
    case MASTER_MODE:
      if(current_switch_value != prev_switch_value && current_switch_value == 1) {
        //  switched the button from off to on
        current_switch_state = ON;
        prev_switch_value = 1;
      }else if(current_switch_value != prev_switch_value && current_switch_value == 0) {
        //  switched the button from on to off
        current_switch_state = OFF;
        prev_switch_value = 0;
      }
      switch_light();

      break;
    case SLAVE_MODE:
    default:
      current_switch_state = SWITCH_States::OFF;
      for(int i = 1; i <= 512; i++) {
        dmx_transceiver->set_dmx_value(i, dmx_transceiver->get_dmx_value(i));
      }
      break;

  }

  dmx_transceiver->transmit();
}



