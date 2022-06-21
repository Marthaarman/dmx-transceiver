#include "dmx.h"
#include "dmx_transceiver.h"
#include "dmx_receiver.h"
#include <Arduino.h>

DMX_Receiver::DMX_Receiver() {

}

void DMX_Receiver::init() {
  Serial.println("init receiver");
}

void DMX_Receiver::start() {
  this->_flag_started = true;
}

void DMX_Receiver::stop() {
  this->_flag_started = false;
}

void DMX_Receiver::iterate() {
  if(this->_flag_started == false) {
    return;
  }
  // dmx_state next_state = this->_dmx_state;
  // bool _bit_value;
  // switch(this->_dmx_state) {
  //   case DMX_States::pause:
  //     if(_bit_value == 0) {
  //       //  we have reached a mark after break
  //       //  move to mark after break and wait for a 2 bit series high followed by a single low
  //       next_state = DMX_States::mark_after_pause;
  //       this->_bit_counter = 0;
  //     }
  //     break;

  //   case DMX_States::mark_after_pause:
  //     //  wait for two high's and one low
  //     if(_bit_value == 1) {
  //       //  two high reached for mark after pause
  //       //  only increase counter, nothing else
  //       this->_bit_counter++;
  //     }else if(this->_bit_counter > 1 && _bit_value == 0) {
  //       //  low after two high reached for mark after pause
  //       next_state = DMX_States::frame;
  //     }else {
  //       //  waiting till mark after pause goes high again
  //       //  do nothing
  //     }
  //     break;
  // }
}