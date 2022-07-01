#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"
#include "dmx.h"

// uint16_t bit_counter = 0;
DMX_Transceiver *dmx_transceiver;

uint16_t _BAUDRateRegister(uint32_t desiredBAUDRate) {
  //  https://onlinedocs.microchip.com/pr/GUID-EC8D3BAB-0B5E-454F-AB6E-6A7C91C6F103-en-US-3/index.html?GUID-1D3F2CB1-9AD7-4AB5-8C5F-9A0926D90992
  //  BAUD = f_osc / (16 * (UBBRn + 1))
  //  UBBRn = ( f_osc / (16 * BAUD) ) - 1
  return ( (F_CPU / (16 * (desiredBAUDRate)) ) - 1 );
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  
  
  

  
}


void loop() {
  // put your main code here, to run repeatedly:
  
  // dmx_transceiver->set_dmx_value(1, 100);
  // dmx_transceiver->transmit();
  // delay(1000);
  // dmx_transceiver->set_dmx_value(1, 0);
  // dmx_transceiver->transmit();
  // delay(1000);
  



  dmx_transceiver->receive();
  
 
    
  if(dmx_transceiver->get_dmx_value(1) == 100) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  }
    
    
  
  delay(10);
  
  // USART_Write_Byte((uint8_t) 0xFF);
}



