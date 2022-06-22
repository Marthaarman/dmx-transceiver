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

void DMX_Transmitter::_build_packet() {
  dmx_state _next_dmx_state = this->_dmx_state;

  switch(this->_dmx_state) {
    case DMX_States::BREAK:
    default:
      // Serial.println("BREAK");
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
      // Serial.println("MAB");
      //  mark-after-break
      //  2 pulses low
      //  when done continue to the start code
      this->_bit_value = LO;
      this->_pulse_counter++;
      if(this->_pulse_counter >= 2) {
        this->_pulse_counter = 0;
        _next_dmx_state = DMX_States::SC;
      }
      break;
    
    case DMX_States::SC:
      // Serial.println("SC");
      //  start code
      //  11 pulses/bits
      //  1 bit high
      //  8 bit data byte 0x00
      //  2 bit high
      this->_pulse_counter++;
      if(this->_pulse_counter == 10 || this->_pulse_counter == 11) {
        this->_bit_value = HI;
      }else {
        this->_bit_value = LO;
      }
      

      
      if(this->_pulse_counter >= 11) {
        this->_pulse_counter = 0;
        this->_slot_counter = 1;
        _next_dmx_state = DMX_States::MTBF;
      }
      break;

    case DMX_States::MTBF:
      // Serial.println("MTBF");
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
      if(this->_pulse_counter == 10 || this->_pulse_counter == 11) {
        this->_bit_value = HIGH;
      }else if(this->_pulse_counter == 1) {
        this->_bit_value = LO;
      }else {
        this->_bit_value = this->_channel_values[this->_slot_counter] >> this->_bit_counter & 0x0001;
        this->_bit_counter++;
      }
      
      if(this->_pulse_counter >= 11) {
          this->_pulse_counter = 0;
          this->_bit_counter = 0;
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
        this->_flag_transmission = true;
      }
      break;
  }
  
  if(this->_bit_value == 0) {
    this->_data_transmission_bits[this->_data_byte] &= ~(1 << this->_data_bit);
  }else {
    this->_data_transmission_bits[this->_data_byte] |= 1 << this->_data_bit;
  }

  this->_data_total_bits++;

  this->_data_bit++;
  if(this->_data_bit >=8) {
    this->_data_byte++;
    this->_data_bit = 0;
  }

  // this->_write_bit();
  this->_dmx_state = _next_dmx_state;

}

void DMX_Transmitter::_write_bit() {
  //bool bit = this->_data_transmission_bits[this->_data_byte] >> this->_data_bit & 1;
  // if(this->_pin_register_address == DMX_PortBytes::B) {
  //   if(this->_bit_value) {
  //     PORTB |= 1 << this->_pin_register_bit;
  //   }else {
  //     PORTB &= ~(1 << this->_pin_register_bit);
  //   }
  // }else if(this->_pin_register_address == DMX_PortBytes::D) {
  //   if(this->_bit_value) {
  //     PORTD |= 1 << this->_pin_register_bit;
  //   }else {
  //     PORTD &= ~(1 << this->_pin_register_bit);
  //   }
  // }
  if(this->_test) {
    PORTD &= ~(1 << this->_pin_register_bit);
  }else {
    PORTD |= 1 << this->_pin_register_bit;
  }
  this->_test = !this->_test;
}

void DMX_Transmitter::send() {
  this->_transmission_bit = 0;
  this->_data_bit = 0;
  this->_data_byte = 0;
  this->_data_total_bits = 0;
  this->_flag_transmission = false;
  while(!this->_flag_transmission) {
    this->_build_packet();
  }

  uint8_t byte = 0;
  for(int i = 0; i < this->_data_total_bits; i++) {
    byte |= ((this->_data_transmission_bits[this->_data_byte] >> this->_data_bit) & 1);
    this->_data_bit++;
    if(this->_data_bit >= 8) {
      this->_data_bit = 0;
      this->_data_byte++;
      Serial.write(byte);
      byte = 0;
    }else {
      byte <<= 1;
    }
  }
  
  this->_flag_transmission = false;
  
  //while(this->_flag_transmission); //wait till we can send
 
  // while(this->_flag_transmission == false) {
  //   this->_build_packet();
  // }
}

void DMX_Transmitter::iterate_transmission() {
  this->_build_packet();
  // if(this->_flag_transmission) {
  //   this->_write_bit();
  //   this->_transmission_bit++;
  //   this->_data_bit++;
  //   if(this->_data_bit >=8) {
  //     this->_data_byte++;
  //     this->_data_bit = 0;
  //   }
  //   if(this->_transmission_bit >= this->_data_total_bits) {
  //     this->_data_total_bits = 0;
  //     this->_transmission_bit = 0;
  //     this->_data_byte = 0;
  //     this->_data_bit = 0;
  //     this->_flag_transmission = false;
  //   }
  // }
}