# Fatures to be implemented in both HW and SW

## HW Features

- [x] Connector for debugger/programmer.
- [x] Two isolated power supplies
- [ ] Indicator LEDs for PS.
- [x] "Error LED" - LED blinking error code in case of hard fault.
- [x] USB debugging interface.
- [x] Radio communication on 868Mhz band.
- [ ] OLED display (not for current release).
- [ ] 4 buttons to control display options (not for current release).
- [x] RESET button.
- [x] Switch to select between normal and debug modes(Optional, maybe we can check in SW if PC is connected through USB?).
- [x] HW test points for osciloscope and multimeter (at least for all communication interfaces and anemometer signal)
- [x] 2 External EEPROM memories, one for status, other for measured data log (wind speed, temperature, humidity, timestamp).
- [ ] "Server" usb dongle, which will be used to communicate with main board(common part with main RF interface).

## SW Features

- [x] SW configuration of peripherals.
- [x] FreeRTOS setup on main MCU.
- [ ] HW Abstraction lib, asynchronous read/write through comm interfaces.
- [ ] USB command line interface for debugging.
- [ ] Pulse frequency masurement with peripheral timer.
- [ ] Temperature and humidity measurement with external thermometer.
- [ ] Drivers for CC110L RF transiver.
- [ ] RF communication standard.
- [ ] Standard for UART communication with future expansion board.
- [ ] Bootloader and SW update through RF(?)
- [ ] PC (or Rpi) app to be "server" for main board and read their measurements through RF
