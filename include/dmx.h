#ifndef _DMX_H_
#define _DMX_H_

//  sources:
//  https://learn.sparkfun.com/tutorials/introduction-to-dmx/all
//  dmx512-online.com/dmx512_packet.html
//  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf

//  Transmitter:
//  TX pin, PD1

//  Receiver:
//  RX pin, PD0

//  DMX Protocol states
typedef enum DMX_States {
  BREAK,    //break                         22  pulses
  MAB,      //mark-after-break              2   pulses
  SC,       //start-code                    11  pulses
  MTBF,     //markt-time-between-frames     X   pulses        
  SLOT,     //each payload packet           11  pulses, repeated 512 times    
  MTBP      //mark-time-between-packets     X   pulses

} dmx_state;

typedef enum DMX_DigitalPins {
    TX = 1,
    RX = 0,
    D2 = 2,
    D3 = 3,
    D4 = 4,
    D5 = 5, 
    D6 = 6,
    D7 = 7,
    D8 = 8,
    D9 = 9,
    D10 = 10,
    D11 = 11,
    D12 = 12
} dmx_digitalPin;

typedef enum DMX_PortBytes {
    B,
    C,
    D
} dmx_portByte;

#endif // _DMX_H_