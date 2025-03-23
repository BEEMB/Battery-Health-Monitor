# Assembly Guide

## 1. PCB Assembly
1. Solder all surface-mount components using the provided Gerber files and BOM.
2. Use the Pick and Place file for component placement.
3. Double-check solder joints, especially for the USB-C connector and microcontroller.

## 2. 3D-Printed Housing
1. Print the STL files from the `3D_Models/` folder.
2. Assemble the housing using the provided screws and inserts.

## 3. Firmware Flashing
1. Upload the `Battery_health.ino` sketch to the ATmega328P microcontroller using an Arduino IDE.
2. Connect the device to a battery and power source to test functionality.

## 4. Testing
1. Connect a lithium battery to the device.
2. Verify that the LED bars display the correct State of Charge (SoC) and State of Health (SoH).
3. Use the serial monitor to check detailed battery data.
