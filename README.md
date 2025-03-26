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
3. run the ESP-IDF command to setup the vscode folder
```bash
espIdf.createVsCodeFolder
```
3. Build and flash the firmware to your ESP32 device.


### Components 📦
To differentiate between ESP-IDF components and other components, the former will always have the prefix `sts_`.
#### 1. [sts_common](components/sts_common)
This component contains the common queues, evenbits, and other utilities used by all other components. When adding a new component, make sure to include this component in the `CMakeLists.txt` file.