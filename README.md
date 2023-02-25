# DMX Transceiver library

This library allows the max485 module to be used for both receiving and transmitting dmx "at the same time".
At the same time, meaning that the transceiver class can switch between the two modes easily. Take a look at the example.

## Compatibility
The code has been tested using a arduino mini pro

## Examples
a simple DMX transceiver that alters channel 1 when channel 511 > 250 is found in examples/transceiver

## DMX Transceiver board with button IO
https://github.com/Marthaarman/dmx_interceptor_PCB

# Credits
This code is heavily based on the prior released dmx-serial library by mathertel:
https://github.com/mathertel/DMXSerial