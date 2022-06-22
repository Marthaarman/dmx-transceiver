#ifndef _DMX_TRANSCEIVER_H_
#define _DMX_TRANSCEIVER_H_

#include <stdint.h>
#include "dmx.h"
#include "dmx_transmitter.h"
#include "dmx_receiver.h"

class DMX_Transceiver {
public:
    DMX_Transceiver();
    void init();
    void start();
    void stop();
    void send();
    void set_pins(dmx_digitalPin RX, dmx_digitalPin TX);
    void set_tx_pin(dmx_digitalPin pin);
    void set_rx_pin(dmx_digitalPin pin);

private:
};


extern DMX_Transmitter *_dmx_transmitter;
extern DMX_Receiver    *_dmx_receiver;


#endif