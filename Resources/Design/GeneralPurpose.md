# What this project does?

The general idea is to create a weather station with anemometer support
to allow automated long-term measurements of wind speed (and potentially other parameters)
throughout at least a year.

Additionally the design is aimed to be as modular as possible, so most
of the code and hardware schematics can be reused in future projects.

## How to measure wind speed?

The anemometer used is a simple device with 3 cups attached to their rotor.
By measuring rotational speed of the rotor, we can calculate actual wind speed.

Anemometer produces square wave with variable frequency, as the rotor spins. Frequency of
such signal is easily measurable by timers which are available in almost any uC.

Measurement collection should be done in fixed time intervals, not to often, so we won't
overflow data buffers and with a frequency that allows for accurate and reliable results.

I.e. 1-10 measurement(s) per minute, averaged after 1h, while keeping the maximal and minimal
wind speed registered during this measurement period (maybe take 1% of the lowest and highest speeds?)

## How to collect data?

The idea is to save "live" data to on-board NV memory every 1h or so.
External memory can hold data from a few days, so sudden loss of power will not
erase measurements result.

Additionally all data collected by anemomenter should be synchronized with external
server which would ask the weather station for data every 24h or so. 

Such server can communicate with weather station through RF interface (~900MHz band?).
Transmission speed isn't a concern and this would allow for much larger signal range than i.e. bluetooth or wifi.

Any PC can be a server, but RPi board(or similar) would be best choice due to low power requirements.
Additional RF "dongle" module needs to be built to allow communication with Pi.

## USB RF Dongle?

(Need to check if such dongle isn't available commercially) A device that converts RF signal to serial data, which can be handled by userspace application.

Creating such device shouldn't be a big concern because it should be enough to isolate a part of already designed PCB and
software responsible for RF interface.
