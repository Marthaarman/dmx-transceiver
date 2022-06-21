#ifndef _DMX_TRANSCEIVER_H_
#define _DMX_TRANSCEIVER_H_

#include <stdint.h>
#include "dmx.h"
#include "dmx_transmitter.h"
#include "dmx_receiver.h"



extern bool LO;
extern bool HI;


class DMX_Transmitter {
public:
    DMX_Transmitter();
    void init();
    void set_pin(dmx_digitalPin pin);
    void iterate();
    void set_dmx_value();
    void start();
    void stop();
private:
    dmx_state _dmx_state = DMX_States::BREAK;

    uint32_t _bit_counter = 0;
    uint32_t _pulse_counter = 0;
    uint32_t _slot_counter = 0;
    uint8_t _channel_values[513];

    bool _flag_started = false;
    bool _flag_data = false;
    bool _bit_value = false;

    void _write_bit();

    dmx_portByte _pin_register_address;
    uint8_t _pin_register_bit;

    bool test = false;
};

class DMX_Receiver {
    public:
    DMX_Receiver();
    void init();
    void set_pin(dmx_digitalPin pin);
    void iterate();
    void start();
    void stop();
private:
    dmx_state _dmx_state = DMX_States::BREAK;
    uint32_t _bit_counter = 0;
    bool _flag_started = false;
};

class DMX_Transceiver {
public:
    DMX_Transceiver();
    void init();
    void start();
    void stop();
    void set_pins(dmx_digitalPin RX, dmx_digitalPin TX);
    void set_tx_pin(dmx_digitalPin pin);
    void set_rx_pin(dmx_digitalPin pin);

private:
};


extern DMX_Transmitter *_dmx_transmitter;
extern DMX_Receiver    *_dmx_receiver;


#endif