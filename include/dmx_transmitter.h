#ifndef _DMX_TRANSMITTER_H_
#define _DMX_TRANSMITTER_H_

class DMX_Transmitter {
public:
    DMX_Transmitter();
    void init();
    void set_pin(dmx_digitalPin pin);
    void iterate();
    void set_dmx_value();
    void start();
    void stop();
    void send();
    void iterate_transmission();
private:
    dmx_state _dmx_state = DMX_States::BREAK;

    uint8_t _bit_counter = 0;
    uint8_t _pulse_counter = 0;
    uint16_t _slot_counter = 0;
    uint16_t _data_total_bits = 0;
    uint8_t _data_bit = 0;
    uint8_t _data_byte = 0;
    uint8_t _data_transmission_bits[5];
    uint16_t _transmission_bit = 0;
    uint8_t _channel_values[513];

    //bool _transmission_bits[8192];
    bool _flag_transmission = false;
    bool _flag_started = false;
    bool _flag_data = false;
    bool _bit_value = false;

    bool _test = false;
    void _write_bit();
    void _build_packet();

    dmx_portByte _pin_register_address;
    uint8_t _pin_register_bit;

    bool test = false;
};

#endif // _DMX_TRANSMITTER_H_