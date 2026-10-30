#include <Arduino.h> 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_BMP280.h> 
 
#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
  #define TFT_CS         14
  #define TFT_RST        15
  #define TFT_DC         32

#elif defined(ESP8266)
  #define TFT_CS         4
  #define TFT_RST        16                                            
  #define TFT_DC         5

#else
  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        4
  #define TFT_RST        6 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         5
#endif
 
//Pin definitions for the joystick
#define JOY_X 15 
#define JOY_Y 16 
#define JOY_SW 17 
 
//Pin definitions for BMP280 sensor
#define I2C_SDA 8 
#define I2C_SCL 9 
 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); 

Adafruit_BMP280 bmp; 

float p = 3.1415926;

void setup(void) { 
  Serial.begin(115200);
  Serial.println("Initializing...");
  tft.init(240, 320); // Initialize the TFT display with width and height

  pinMode(JOY_SW, INPUT_PULLUP); // Set the joystick switch pin as input with pull-up resistor
} 

void loop() { 

}
 