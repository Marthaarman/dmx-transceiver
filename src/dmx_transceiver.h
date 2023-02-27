//  ---
//  DMX Transceiver
//  By Mart Haarman
//  https://github.com/Marthaarman/dmx-transceiver
//  code based on DMX Serial library by matthias hertel
//  https://github.com/mathertel/DMXSerial/
//
//  Other sources:
//  https://learn.sparkfun.com/tutorials/introduction-to-dmx/all
//  http://dmx512-online.com/dmx512_packet.html
//  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
//  ---

#ifndef _DMX_TRANSCEIVER_H_
#define _DMX_TRANSCEIVER_H_

#include <stdint.h>
#include "dmx_transmitter.h"
#include "dmx_receiver.h"

class DMX_Transceiver {
public:
    //  constructor
    DMX_Transceiver();

    //  sets new instances for the transmitter and receiver
    void init();

    //  functions that call the transmitter, receiver or both combined
    void transmit();        //  transmitter
    void receive();         //  receiver
    void transceive();      //  both

    //  set which pins are used for the dmx modules
    void set_tx_enable_pin(uint8_t pin);
    void set_rx_enable_pin(uint8_t pin);

    //  set RX hold time (pause time)
    void set_rx_timeout(uint16_t time_milliseconds);
    void set_rx_hold_time(uint16_t time_milliseconds) {set_rx_timeout(time_milliseconds);}

    //  set a dmx value for a specific channel
    //  will be written when transmit or transceive function is called
    void set_dmx_value(uint16_t channel, uint8_t value);

    //  gets the dmx value
    //  will return 0 if receive is never called
    //  receive or transceive needs to be called to read (receive) newest dmx value
    uint8_t get_dmx_value(uint16_t channel); 
};


extern DMX_Transmitter *_dmx_transmitter;
extern DMX_Receiver    *_dmx_receiver;

#endif