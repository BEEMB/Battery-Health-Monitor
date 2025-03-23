# Battery Health Monitor

This project is a compact device for monitoring the **State of Health (SoH)** and **State of Charge (SoC)** of lithium batteries. It uses the BQ27441 fuel gauge IC and BQ24075 charger IC, along with an ATmega328P microcontroller, to provide real-time battery data and display it through LED bars.

## Features
- Real-time monitoring of battery State of Health (SoH) and State of Charge (SoC).
- LED indicators for SoH and SoC.
- USB-C compatibility for charging and power input.
- Multiple battery connection options (18650 holder, JST connector, screw terminal).
- Compact and self-contained design.

## Hardware
- **Schematic**: [Schematic Charging Circuit.pdf](Hardware/Schematic/Schematic%20Charging%20Circuit.pdf) | [Schematic MCU Circuit.pdf](Hardware/Schematic/Schematic%20MCU%20Circuit.pdf)
- **Bill of Materials**: [BOM_Battery health device.xlsx](Hardware/BOM/BOM_Battery%20health%20device.xlsx)
- **Gerber Files**: [Gerber_Files/](Hardware/Gerber_Files/)
- **Pick and Place File**: [PickAndPlace_PCB_Battery-health-device.xlsx](Hardware/Pick_and_Place/PickAndPlace_PCB_Battery-health-device.xlsx)
- **3D Models**: [3D_Models/](Hardware/3D_Models/)

## Firmware
- **Arduino Code**: [Battery_health.ino](Firmware/Battery_health.ino)

## Assembly Guide
For step-by-step assembly instructions, refer to the [Assembly Guide](Assembly_Guide.md).

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Video Demonstration
Watch the video demonstration of this project on [YouTube](https://www.youtube.com/your-video-link).

## Credits
- Inspired by SparkFun's Battery Babysitter.
- Designed and built by [Your Name].
