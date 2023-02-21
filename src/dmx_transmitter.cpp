#include "dmx_transceiver.h"
#include "dmx_transmitter.h"
#include "dmx_boards.h"
#include <Arduino.h>

//  TRANSMITTER

DMX_Transmitter::DMX_Transmitter() {}

void DMX_Transmitter::init() {
  this->_DMX_BREAK_BAUD_REG = this->_BAUDRateRegister(this->_DMX_BREAK_BAUD);
  this->_DMX_PAYLOAD_BAUD_REG = this->_BAUDRateRegister(this->_DMX_PAYLOAD_BAUD);
}

void DMX_Transmitter::_start() {
  this->_byte_counter = 0;
  this->_flag_started = true;
  this->_flag_send = true;
}

void DMX_Transmitter::_stop() {
  // digitalWrite(this->_tx_enable_pin, LOW);
  this->_flag_started = false;
  this->_flag_send = false;
  this->_USART_Stop();
}

void DMX_Transmitter::set_enable_pin(uint8_t pin) {
  this->_tx_enable_pin = pin;
  pinMode(pin, OUTPUT);
  digitalWrite(this->_tx_enable_pin, HIGH);
}

void DMX_Transmitter::set_dmx_value(uint16_t channel, uint8_t value) {
  this->_channel_values[channel] = value;
}


uint16_t DMX_Transmitter::_BAUDRateRegister(uint32_t desiredBAUDRate) {
  //  https://onlinedocs.microchip.com/pr/GUID-EC8D3BAB-0B5E-454F-AB6E-6A7C91C6F103-en-US-3/index.html?GUID-1D3F2CB1-9AD7-4AB5-8C5F-9A0926D90992
  //  BAUD = f_osc / (16 * (UBBRn + 1))
  //  UBBRn = ( f_osc / (16 * BAUD) ) - 1
  return ( (F_CPU / (16 * (desiredBAUDRate)) ) - 1 );
}

void DMX_Transmitter::transmit() {
  //  set flags
  //  enable transmission pin
  this->_start();

  //  initiate the break
  this->_USART_Init_BREAK();

  //  write start code which is the first value of the pointer
  this->_USART_Write_BREAK();

  //  add a delay to make sure packet is sent
  //  packet is measured to take about 25ms
  delay(50);
}

void DMX_Transmitter::_USART_Stop() {
  DMX_UCSRB = 0;
}

void DMX_Transmitter::_USART_Init_BREAK() {
  //  set the baud rate register
  DMX_UBRRH = (this->_BAUDRateRegister(100000) >> 8);
  DMX_UBRRL = (this->_BAUDRateRegister(100000));

  //  enable Transmitter and Transmitter interrupts
  DMX_UCSRB = ( (1 << DMX_TXEN) | (1 << DMX_TXCIE) );

  //  set frame format (8 data, even parity and 2 stop bits)
  DMX_UCSRC = SERIAL_8E2;
}

void DMX_Transmitter::_USART_Init_DMX() {
  //  set the baud rate register
  DMX_UBRRH = (this->_BAUDRateRegister(250000) >> 8);
  DMX_UBRRL = (this->_BAUDRateRegister(250000));

  //  enable Transmitter and Transmitter interrupts
  DMX_UCSRB = ( (1 << DMX_TXEN) | (1 << DMX_TXCIE) );

  //  set frame format (8 data, even parity and 2 stop bits)
  DMX_UCSRC = SERIAL_8N2;
}

void DMX_Transmitter::_USART_Write_BREAK() {
  //  break is 88us low
  //  write zeros
  //  write the payload to UDR0
  DMX_UDR = (uint8_t) 0;
}

void DMX_Transmitter::_USART_Write_BYTE(uint8_t BYTE) {
  //  write the payload byte -> BYTE
  //  set UDR0 to the value of the byte
  DMX_UDR = BYTE;
}

void DMX_Transmitter::interrupt() {
  //  interrupt is public
  //  calls internal private interrupt
  this->_USART_TX_Interrupt();
}

void DMX_Transmitter::_USART_TX_Interrupt() {
  //  private interrupt function for the transmitter
  //  if send is called, allow either payload or start code
  //  if send flag is set to false, the interrupt of the last sent byte will still be triggered
  if(this->_flag_send) {
    if(this->_flag_payload) {
      //  in payload phase
      //  send next byte each iteration
      //  DMX has 512 packets (bytes)
      this->_USART_Write_BYTE(this->_channel_values[this->_byte_counter]);
      this->_byte_counter++;
    }else {
      //  in startcode phase
      //  enable the DMX speeds of 250k baud
      //  send first byte of the DMX values which cannot be overwritten and equals 0
      //  continue to the payload after
      this->_USART_Init_DMX();
      this->_flag_payload = true;
      this->_USART_Write_BYTE(this->_channel_values[this->_byte_counter]);
      this->_byte_counter++;
    }
  }else {
    //  last byte has been sent
    //  reset all flags
    //  disable the TX pin
    this->_stop();
  }
  if(this->_byte_counter >= 513) {
    //  sent last byte
    //  reset all flags
    this->_byte_counter = 0;
    this->_flag_payload = false;
    this->_flag_send = false;
  }
}