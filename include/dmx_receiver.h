#ifndef _DMX_RECEIVER_H_
#define _DMX_RECEIVER_H_

class DMX_Receiver {
    public:

    //  constructor
    DMX_Receiver();

    //  initializes the transmitter
    //  calculates the baud rate
    void init();

    //  set the pin that is enabled during receiving
    //  this pin is set to high when enabled
    //  used for devices like the max485
    //  default pin 3
    void set_enable_pin(uint8_t pin);

    //  allow the device to receive DMX frames
    //  while called, this function will idle other processing with exception of interrupts
    //  stores received dmx payload
    void receive();

    //  called when a byte has been received
    void interrupt();

    //  returns the value of a specific dmx channel
    uint8_t get_dmx_value(uint16_t channel); 
    
private:
    
    uint8_t _rx_enable_pin = 6;
    uint8_t *_channel_values = (uint8_t*) calloc(513, sizeof(uint8_t));

    uint16_t _byte_counter = 0;

    uint32_t _bit_counter = 0;

    uint32_t _DMX_BREAK_BAUD = 100000;
    uint32_t _DMX_PAYLOAD_BAUD = 250000;
    uint32_t _DMX_BREAK_BAUD_REG;
    uint32_t _DMX_PAYLOAD_BAUD_REG;

    bool _flag_started = false;
    bool _flag_transceive = false;
    bool _flag_payload = false;
    bool _flag_break_received = false;
    bool _flag_receive = false;

    

    void _start();
    void _stop();

    void _USART_Init_BREAK();
    void _USART_Init_DMX();
    void _USART_Read_BREAK();
    void _USART_Read_BYTE(uint8_t byte);
    void _USART_Stop();
    void _USART_RX_Interrupt();

    uint16_t _BAUDRateRegister(uint32_t desiredBAUDRate);
};

#endif // _DMX_RECEIVER_H_