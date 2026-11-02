# Embedded Weather Station & User Interface System

## Project Overview

This project is a environment monitoring and interface system based on ESP32-S3.It interfaces ESP32-S3 microcontroller with a BMP280 sensor(I2C), an SPI TFT display(ST7789), and a KY-023 joystick to create this weather station.

The system features the user to navigate the menu using the joystick, displaying real-time atmospheric pressure( hPa and mbar) and temperature. Also, it includes a bonus elevation tracking feature allowing users to adjust the sea-level pressure reference.


## Hardware Architecture & Pin Assignments 

* **Microcontroller:** ESP32-S3-WROOM-2 
* **Sensor:** BMP280 Barometric Pressure and Temperature Sensor 
* **Display:** GMT020-02-7P 2.0-inch SPI TFT LCD (ST7789 Driver) 
* **User Input:** KY-023 Dual-Axis Analog Joystick with Push-Button

| Component | Pin Name | ESP32-S3 GPIO | Function | 
| :--- | :--- | :--- | :--- | 
| **TFT Display** |
| | CS | GPIO 4 | SPI Chip Select | 
| | DC | GPIO 5 | Data/Command | 
| | RST | GPIO 6 | Reset | 
| | SDA (MOSI) | GPIO 11 | SPI Data | 
| | SCL (SCLK) | GPIO 12 | SPI Clock | 
| **BMP280** |
| | SDA | GPIO 8 | I2C Data | 
| | SCL | GPIO 9 | I2C Clock | 
| **Joystick** | 
| | VRx | GPIO 15 | Analog X-axis (Adjust Elevation) | 
| | VRy | GPIO 16 | Analog Y-axis (Menu Navigation) | 
| | SW | GPIO 17 | Push Button (Select/Exit) | 

## Software Architecture

The firmware is developed using PlatformIO in VS Code. It adheres to modular design principles: 
* **Non-blocking Logic:** Uses `millis()` instead of `delay()` for real-time data refreshing and UI transitions, ensuring the joystick remains responsive. 
* **State Machine:** `currentPage` variables manage transitions between the main menu and data display pages. 
* **Debouncing:** Hysteresis thresholds (e.g., `yValue < 500` or `> 3500`) and state tracking (`lastJoyY`) prevent accidental multi-scrolling from a single joystick flick. 

## Engineering Troubleshooting

1. **ESP32-S3-WROOM-2 Flash Configuration Crash:**  

   Initially, the module suffered from a continuous boot loop, throwing an `assert failed: do_core_init startup.c:328` error. This was caused by a mismatch between the default board configuration and the actual hardware capabilities.
   
   **Resolution:** Modified the `platformio.ini` file to explicitly define the correct memory types, setting `board_build.psram_type = opi` and allocating `32MB Flash`. This completely resolved the kernel panic and allowed the system to boot normally.

2. **SPI Bus Congestion & System Crash (Memory/Bus Bottleneck):**  

  While implementing the joystick-based elevation adjustment feature, high-frequency screen redrawing (`drawDataPage()`) caused the SPI bus to become overwhelmed. This led to chip crashes and unexpected white screens.  
  
  **Resolution:** Introduced a boolean state flag (`isAdjusting`). The screen is now only redrawn when the joystick value actually changes (edge detection) or via a non-blocking `millis()` timer. This "redraw-on-demand" approach successfully eliminated bus congestion and prevented system crashes. 

## How to Run 

1. Clone the repository. 
2. Open the project in VS Code with PlatformIO installed. 
3. Build and upload to the ESP32-S3 board. 
4. Use the joystick to navigate the menu (Up/Down to select, Press to enter). 
5. On the Elevation page, use Left/Right to adjust the sea-level pressure reference. 
   
