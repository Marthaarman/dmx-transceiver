#include "dmx.h"
#include "dmx_transceiver.h"
#include "dmx_receiver.h"
#include <Arduino.h>

DMX_Receiver::DMX_Receiver() {}

void DMX_Receiver::init() {
  this->_DMX_BREAK_BAUD_REG = this->_BAUDRateRegister(this->_DMX_BREAK_BAUD);
  this->_DMX_PAYLOAD_BAUD_REG = this->_BAUDRateRegister(this->_DMX_PAYLOAD_BAUD);
}

uint16_t DMX_Receiver::_BAUDRateRegister(uint32_t desiredBAUDRate) {
  //  https://onlinedocs.microchip.com/pr/GUID-EC8D3BAB-0B5E-454F-AB6E-6A7C91C6F103-en-US-3/index.html?GUID-1D3F2CB1-9AD7-4AB5-8C5F-9A0926D90992
  //  BAUD = f_osc / (16 * (UBBRn + 1))
  //  UBBRn = ( f_osc / (16 * BAUD) ) - 1
  return ( (F_CPU / (16 * (desiredBAUDRate)) ) - 1 );
}

void DMX_Receiver::_start() {
  this->_byte_counter = 0;
  this->_flag_started = true;
  this->_flag_receive = true;
}

void DMX_Receiver::_stop() {
  digitalWrite(this->_rx_enable_pin, LOW);
  this->_flag_started = false;
  this->_flag_receive = false;
  this->_USART_Stop();
  // digitalWrite(LED_BUILTIN, LOW);
}

void DMX_Receiver::set_enable_pin(uint8_t pin) {
  this->_rx_enable_pin = pin;
  pinMode(pin, OUTPUT);
}

void DMX_Receiver::receive() {
  //  set flags
  //  enable receive pin
  this->_start();

  //  initiate the break
  this->_USART_Init_DMX();

  //  stall main code to make sure full packet is received
  while(this->_flag_started) { if(this->_flag_receive == false) {break; }};
}

void DMX_Receiver::_USART_Stop() {
  UCSR0B = 0;
}

void DMX_Receiver::_USART_Init_DMX() {
   //  set the baud rate register
  UBRR0H = (this->_BAUDRateRegister(250000) >> 8);
  UBRR0L = (this->_BAUDRateRegister(250000));

  //  enable Transmitter (TXEN0) and Transmitter interrupts (TXCIE0)
  UCSR0B = ( (1 << RXEN0) | (1 << RXCIE0) );

  //  set frame format (8 data, even parity and 2 stop bits)
  UCSR0C = SERIAL_8N2;
}


void DMX_Receiver::_USART_RX_Interrupt() {
  //  get frame error
  uint8_t frame_error = (UCSR0A & (1 << FE0)); 

  //  get the byte of data
  uint8_t frame = UDR0;

  if(this->_flag_receive) {
    if(frame == 0 && this->_byte_counter == 0 && frame_error != 0) {
      this->_flag_break_received = true;
    } else if(frame == 0 && this->_byte_counter == 0 && frame_error == 0 && this->_flag_break_received) {
      //  start code most likely
      // digitalWrite(LED_BUILTIN, HIGH);
      this->_byte_counter++;
    }else  if(this->_byte_counter > 0) {
      this->_channel_values[this->_byte_counter] = frame;
      this->_byte_counter++;
    }
  }
  if(this->_byte_counter >= 512) {
    this->_stop();
    this->_flag_payload = false;
    this->_flag_receive = false;
    this->_flag_break_received = false;
    this->_byte_counter = 0;
    // digitalWrite(LED_BUILTIN, LOW);
    
  }
}

void DMX_Receiver::interrupt() {
  this->_USART_RX_Interrupt();
}

uint8_t DMX_Receiver::get_dmx_value(uint8_t channel) {
  return this->_channel_values[channel];
}