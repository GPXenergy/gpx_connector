# GPX Connector

The GPX-Connector is part of the GPX system to connect a smart meter to the GPX 
multi-user system. It runs on an ESP32, which connects to the smart meter 
through a RJ11 cable and sends the data over wifi to the GPX API. 

## Local development setup

The project runs on [PlatformIO](https://platformio.org/), an open source 
framework and IDE for embedded development. 

This project has 2 platformio environments:
* __GPXConnector__ (default) - The final device / prototypes to run the system
* __wrover__ - The ESP-WROVER-kit device, used for debugging

Some platformio commands:
* `pio run`
  * Compile the program on default environment (GPXConnector)
* `pio run -t upload -e GPXConnector`
  * Compile and upload the program for specific environment
* `pio test -e wronver`
  * Run all the unit tests on the wrover environment

