#include "dmx.h"
#include "dmx_transceiver.h"
#include "dmx_transmitter.h"
#include <Arduino.h>

//  TRANSMITTER

DMX_Transmitter::DMX_Transmitter() {
    
}

void DMX_Transmitter::init() {
    Serial.println("init transmitter");
}

void DMX_Transmitter::start() {
  this->_flag_started = true;
}

void DMX_Transmitter::stop() {
  this->_flag_started = false;
}

void DMX_Transmitter::set_pin(dmx_digitalPin pin) {
  switch(pin) {
    case DMX_DigitalPins::TX:
    case DMX_DigitalPins::RX:
    case DMX_DigitalPins::D2:
    case DMX_DigitalPins::D3:
    case DMX_DigitalPins::D4:
    case DMX_DigitalPins::D5:
    case DMX_DigitalPins::D6:
    case DMX_DigitalPins::D7:
      this->_pin_register_address = DMX_PortBytes::D;
      this->_pin_register_bit = (uint8_t)pin;
      DDRD |= 1 << this->_pin_register_bit;
      Serial.print(this->_pin_register_bit);
      Serial.println(this->_pin_register_address);
    break;

    case DMX_DigitalPins::D8:
      this->_pin_register_bit = 0;
      this->_pin_register_address = DMX_PortBytes::B;
      DDRB |= 1;
    break;

    case DMX_DigitalPins::D9:
      this->_pin_register_bit = 1;
      this->_pin_register_address = DMX_PortBytes::B;
      DDRB |= 1 << this->_pin_register_bit;
    break;

    case DMX_DigitalPins::D10:
      this->_pin_register_bit = 2;
      this->_pin_register_address = DMX_PortBytes::B;
      DDRB |= 1 << this->_pin_register_bit;
    break;

    case DMX_DigitalPins::D11:
      this->_pin_register_bit = 3;
      this->_pin_register_address = DMX_PortBytes::B;
      DDRB |= 1 << this->_pin_register_bit;
    break;

    case DMX_DigitalPins::D12:
      this->_pin_register_bit = 4;
      this->_pin_register_address = DMX_PortBytes::B;
      DDRB |= 1 << this->_pin_register_bit;
    break;
  }
}

void DMX_Transmitter::iterate() {
  dmx_state _next_dmx_state = this->_dmx_state;
  if(this->_flag_started == false) {
    return;
  }
  switch(this->_dmx_state) {

    case DMX_States::BREAK:
    default:
      //  BREAK
      //  22 pulses high
      //  when done, continue to mark after break
      this->_bit_value = LO;
      this->_pulse_counter++;
      if(this->_pulse_counter >= 22) {
        this->_pulse_counter = 0;
        _next_dmx_state = DMX_States::MAB;
      }
      break;

    case DMX_States::MAB:
      //  mark-after-break
      //  2 pulses low
      //  when done continue to the start code
      this->_bit_value = HI;
      this->_pulse_counter++;
      if(this->_pulse_counter >= 2) {
        this->_pulse_counter = 0;
        _next_dmx_state = DMX_States::SC;
      }
      break;
    
    case DMX_States::SC:
      //  start code
      //  11 pulses/bits
      //  1 bit high
      //  8 bit data byte 0x00
      //  2 bit high

      this->_pulse_counter++;
      switch(this->_pulse_counter) {
        case 1:
          this->_bit_value = LO;
          break;
        case 10:
        case 11:
          this->_bit_value = HI;
          break;
        default:
          this->_bit_value = LO;
          break;
      }

      
      if(this->_pulse_counter >= 11) {
        this->_pulse_counter = 0;
        this->_slot_counter = 1;
        _next_dmx_state = DMX_States::MTBF;
      }
      break;

    case DMX_States::MTBF:
      //  mark time between frames
      //  just give 4 pulses
      this->_bit_value = HI;
      this->_pulse_counter++;
      if(this->_pulse_counter >= 4) {
        this->_pulse_counter = 0;
        _next_dmx_state = DMX_States::SLOT;
      }
      break;
    
    case DMX_States::SLOT:
      //  512 slots are being sent
      this->_pulse_counter++;
      switch(this->_pulse_counter) {
        case 1:
          this->_bit_value = LO;
          break;
        case 10:
        case 11:
          this->_bit_value = HI;
          break;
        default:
          this->_bit_value = this->_channel_values[this->_slot_counter] >> this->_bit_counter & 0x0001;
          this->_bit_counter++;
          break;
      }
      
      if(this->_pulse_counter >= 11) {
          this->_pulse_counter = 0;
          this->_bit_counter = LO;
          this->_slot_counter++;
          _next_dmx_state = DMX_States::MTBF;
      }

      if(this->_slot_counter >= 513) {
        this->_pulse_counter = 0;
        this->_bit_counter = 0;
        this->_slot_counter = 0;
        _next_dmx_state = DMX_States::MTBP;
      }
      break;
    case DMX_States::MTBP:
      //  mark time between frames
      //  just give 4 pulses high as mark
      this->_bit_value = HI;
      this->_pulse_counter++;
      if(this->_pulse_counter >= 4) {
        this->_pulse_counter = 0;
        _next_dmx_state = DMX_States::BREAK;
      }
      break;
  }
  this->_write_bit();
  this->_dmx_state = _next_dmx_state;
}

void DMX_Transmitter::_write_bit() {
  if(this->_pin_register_address == DMX_PortBytes::B) {
    if(this->_bit_value) {
      PORTB |= 1 << this->_pin_register_bit;
    }else {
      PORTB &= ~(1 << this->_pin_register_bit);
    }
  }else if(this->_pin_register_address == DMX_PortBytes::D) {
    if(this->_bit_value) {
      PORTD |= 1 << this->_pin_register_bit;
    }else {
      PORTD &= ~(1 << this->_pin_register_bit);
    }
  }
}
