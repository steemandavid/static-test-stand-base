# Static Test Stand (Base) 🚀

This project is the base station code for a static test stand, built using:
- 💡 **ESP32 family SoCs**
- 🛠️ **ESP-IDF v5.4.0**. 

## Features ✨
- Modular design with multiple ESP-IDF components for various functionalities.
- Designed for reliability and scalability.

## Getting Started 📚
1. Clone the repository.
2. Set up ESP-IDF v5.4.0.
3. run the ESP-IDF command to setup the vscode folder `espIdf.createVsCodeFolder`
3. Build and flash the firmware to your ESP32 device.


### Components 📦
To differentiate between ESP-IDF components and other components, the former will always have the prefix `sts_`.
#### 1. [sts_common](components/sts_common)
This component contains the common queues, evenbits, and other utilities used by all other components. When adding a new component, make sure to include this component in the `CMakeLists.txt` file.

#### 2. [sts_core](components/sts_core)
This component contains the core functionalities of the static test stand. It has the main statemachine and handles the commands received from the remote or console.

#### 3. [sts_console](components/sts_console)
Use the terminal to run commands on the sts_core component. This is usefull for testing the handling of commands by the core component without the need for a remote. Or to use the static test stand in a standalone mode.

#### 4. [sts_measure](components/sts_measure)
This component contains the code to measure the all the sensors connected to the static test stand.
Measurements are taken at a fixed interval and are stored in a queue for further processing.