# This document contains all general notes and requirements to make further design process easier

## Essential elements in system

Essential components which malfunction will cause HardFault.

- External EEPROM memory - it is required to save data log and maybe also log potential faults (error codes with timestamp)

- Radio chip not responding - this is required for communication with Data Server.

## Elements required for full funciotnality

Components which will not cause HardFault, but their lack or malfuncion will be reported by application.

- External anemometer not connected - The basic functionality of project, lack of anemometer probe should also block data log (we don't want to consume EEPROM's cells durability for waste).

- External thermometer not responding - This should be noted in error log (Optional expansion feature)

- No response from Data Server on boot or for more than 48h - Data collected by device should be sent through radio to Data Server, no response for prolonged time should be noted in Error Log, but measurements are saved in NV memory anyway. We can wait such long time thanks to build-in RTC.

## Optional elements

- USB debug interface - This is just for development (as the name suggests), there should be a HW switch to enter this mode on boot or on normal running mode there may be a check for enabled debug interface.

- Expansion module - It's just in case of further development or adding new HW which was not planed at first board iteration.

## Notes

- After config generation remove freertos_app.c and create custom app at project core
