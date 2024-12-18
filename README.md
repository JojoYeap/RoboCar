# RoboCard: ESP32CAM on Wheels

RoboCard is a compact robot car powered by the ESP32CAM module, featuring a chassis ingeniously crafted from expired credit cards. This project combines sustainability with robotics, offering an innovative approach to DIY electronics.

<img src="https://jojoyeap.github.io/RoboCard/images/Car_Main.jpg" alt="RoboCard" width="300">

## Introduction

Welcome to the RoboCard project! This tutorial guides you through building a robot car using an ESP32CAM and a custom frame made from expired credit cards. Designed for beginners, it provides step-by-step instructions to help you grasp the fundamentals of robotics. By the end, you'll have a fully functional robot car that can be remotely controlled and capture images with its integrated camera.

## Features

- **Sustainable Design**: Utilizes expired credit cards for the chassis, promoting recycling and creativity.
- **ESP32CAM Integration**: Leverages the capabilities of the ESP32CAM for control and image capture.
- **Remote Control**: Operable via a custom-developed app for seamless user interaction.

## Bill of Materials

To build RoboCard, you'll need the following components:

- **ESP32CAM**: Wireless IoT vision development board.
- **Mini Breadboard**: 170 holes, 35mm x 42mm.
- **Jumper Wires**: Male-to-male connectors.
- **TB6612FNG Motor Driver**: Dual-channel 1.2A motor driver with presoldered header.
- **DC Micro Metal Gear Motors**: 6V, 440RPM.
- **Rear Wheels**: Mini wheel, 34x7mm, compatible with N20 gear motor.
- **Front Wheel**: W420 steel ball universal wheel caster.
- **USB to UART Converter**: FTDI-based converter for programming.
- **Small Power Bank**: Any power bank with a microUSB cable.
- **Expired Credit Cards**: Two cards for the chassis.
- **Foam Material**: Used sponge or any foam for cushioning.
- **Tools**: Soldering iron, tape, and your tinkering hat!

## Assembly Instructions

Follow these steps to assemble your RoboCard:

1. **Mount Components on Breadboard**: Attach the ESP32CAM and motor driver to the mini breadboard.
2. **Wire Connections**: Connect the ESP32CAM to the motor driver using jumper wires as specified.
3. **Power Connections**: Link the ESP32CAM and motor driver to the power rails appropriately.
4. **Prepare Motors**: Attach wheels to the motors and solder the necessary wires.
5. **Construct Chassis**: Assemble the expired credit cards into a frame, securing the motors and front wheel.
6. **Install Electronics**: Mount the breadboard and power bank onto the chassis.

For detailed instructions and images, refer to the [RoboCard Tutorial](https://jojoyeap.github.io/RoboCard/).

## Programming the ESP32CAM

To program the ESP32CAM, you'll need the Arduino IDE with the ESP32 board package installed.

1. **Install ESP32 Board Package**: Follow the instructions to add ESP32 support to your Arduino IDE.
2. **Connect USB to UART Converter**: Interface the ESP32CAM with your computer using the USB to UART converter.
3. **Upload Code**: Open the provided Arduino sketch and upload it to the ESP32CAM.

Ensure all connections are secure before powering the device.

## Contributing

Contributions are welcome! Feel free to fork this repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

For more information and detailed instructions, visit the [RoboCard Project Page](https://jojoyeap.github.io/RoboCard/).
