#ifndef _DMX_TRANSCEIVER_H_
#define _DMX_TRANSCEIVER_H_

#include <stdint.h>
#include "dmx.h"
#include "dmx_transmitter.h"

class DMX_Transceiver {
public:
    DMX_Transceiver();
    void init();
    void start();
    void stop();
    void transmit();
    void receive();
    void transceive();
    void set_tx_enable_pin(uint8_t pin);
    void set_dmx_value(uint8_t channel, uint8_t value);
    uint8_t get_dmx_value(uint8_t channel); 
private:
};


extern DMX_Transmitter *_dmx_transmitter;
// extern DMX_Receiver    *_dmx_receiver;


#endif