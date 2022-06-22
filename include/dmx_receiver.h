#ifndef _DMX_RECEIVER_H_
#define _DMX_RECEIVER_H_

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

#endif // _DMX_RECEIVER_H_