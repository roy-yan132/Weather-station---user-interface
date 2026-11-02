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

float p = 3.1415926; // Variable to store the value of pi

int menuOption = 0; // Variable to keep track of the selected menu option
int currentPage = 0; // Variable to keep track of the current page being displayed
int lastJoyY = 2048; // Variable to store the last joystick Y position
int lastJoyX = 2048; // Variable to store the last joystick X position
float seaLevelPressure = 1013.25; // Standard sea level pressure in hPa
bool lastButtonState = HIGH; // Variable to store the last button state
unsigned long lastRefresh = 0; // Variable to store the last time the data page was refreshed


void drawMenu(){
  //Start to draw starting menu
  tft.fillScreen(ST77XX_BLACK);//Screen background black

  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20,20);
  tft.print("-----MENU-----");
  String items[] = {"1. Pressure hPa", "2. Pressure mbar", "3. Temp(C)", "4. Elevation(m)"}; 
   
  for (int i = 0; i < 4; i++) { 
    tft.setCursor(20, 65 + (i * 45)); 
    if (i == menuOption) { 
      tft.setTextColor(ST77XX_BLACK, ST77XX_CYAN); //Highlight the selected menu item with cyan background and black text
    } 
    else { 
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK); 
    } 
    tft.print(items[i]); 
  } 
}

void drawDataPage() { 
  tft.fillScreen(ST77XX_BLACK); 
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK); 
   
  float temp = bmp.readTemperature(); 
  float pressure = bmp.readPressure() / 100.0F; 
 
  if (currentPage == 1) { 
    tft.setTextSize(4); 
    tft.setCursor(20, 20); 
    tft.print("Pressure:"); 
    tft.setCursor(20, 100); 
    tft.print(pressure, 1); 
    tft.print(" hPa"); 

    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_RED); 
    tft.setCursor(20, 210); 
    tft.print("Press SW to Exit");

  }  
  else if (currentPage == 2) { 
    tft.setTextSize(6);  
    tft.setCursor(10, 50); 
    tft.print(pressure, 1); 
    tft.setTextSize(3); 
    tft.setCursor(20, 150); 
    tft.print("mbar"); 

    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_RED); 
    tft.setCursor(20, 210); 
    tft.print("Press SW to Exit");
  }  
  else if (currentPage == 3) { 
    tft.setTextSize(4); 
    tft.setCursor(20, 20); 
    tft.print("Temp:"); 
    tft.setCursor(20, 100); 
    tft.print(temp, 1); 
    tft.print(" C"); 

    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_RED); 
    tft.setCursor(20, 210); 
    tft.print("Press SW to Exit");
  }
  else if (currentPage == 4) { 
    tft.setTextSize(4); 
    tft.setCursor(20, 20); 
    tft.print("Elevation:"); 
    tft.setCursor(20, 100); 
    float elevation = bmp.readAltitude(seaLevelPressure); // Calculate elevation based on sea level pressure
    tft.print(elevation, 1); 
    tft.print(" m"); 

    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_YELLOW); 
    tft.setCursor(20, 140); 
    tft.print("Ref: "); 
    tft.print(seaLevelPressure, 1);
    tft.print(" hPa");
    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_CYAN); 
    tft.setCursor(20, 160); 
    tft.print("L/R to adjust"); 
    tft.setTextSize(2); 
    tft.setTextColor(ST77XX_RED); 
    tft.setCursor(20, 210); 
    tft.print("Press SW to Exit");
  } 
} 

void setup(void) { 
  Serial.begin(115200);
  Serial.println("Initializing...");

   // Initialize custom I2C pins for the ESP32-S3
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the BMP280 sensor explicitly at address 0x76
  unsigned status = bmp.begin(0x76);
  //Error checking
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring, "
                     "address, or solder jumpers!"));
    Serial.print(F("SensorID was: 0x")); Serial.println(bmp.sensorID(), 16);
    Serial.println(F("ID of 0xFF typically means a bad connection or incorrect address."));
    while (1) {
      delay(1000);
    }
  }

  // Configure default oversampling and filtering settings
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time */

  Serial.println(F("BMP280 successfully initialized at 0x76!"));

  tft.init(240, 320); // Initialize the TFT display with width and height
  tft.setRotation(1); //Rotate the initial menu

  pinMode(JOY_SW, INPUT_PULLUP); // Set the joystick switch pin as input with pull-up resistor

  drawMenu(); // Draw the initial menu

} 

void loop() { 
  int yValue = analogRead(JOY_Y); // Read the Y-axis value of the joystick
  int xValue = analogRead(JOY_X); // Read the X-axis value of the joystick
  int btnValue = digitalRead(JOY_SW); // Read BMP280 Value by pressing the joystick

  if(currentPage == 0){
    //if pull up the joystick
    if (yValue < 500 && lastJoyY >= 500) {  
      menuOption--; //Move up
      if (menuOption < 0) {
        menuOption = 3;//Wrap around to the last option if it goes above the first option
      } 
      drawMenu(); 
    } 
    //if pull down the joystick
    else if (yValue > 3500 && lastJoyY <= 3500) {  
      menuOption++; //Move down
      if (menuOption > 3) {
        menuOption = 0;//Wrap around to the first option if it goes below the last option
      }
      drawMenu(); 
    } 

    if (btnValue == LOW && lastButtonState == HIGH) { 
      currentPage = menuOption + 1;  
      drawDataPage(); 
      delay(300); 
    } 
  } else { 
    // Exit to menu if joystick is pressed
    if (btnValue == LOW && lastButtonState == HIGH) { 
      currentPage = 0;
      drawMenu(); 
      delay(300); 
    }
    bool isAdjusting = false; // Flag to indicate if the user is adjusting the sea level pressure

    if (currentPage == 4) { 
      // Adjust sea level pressure if joystick is moved left or right
      if (xValue < 1000 && lastJoyX >= 1000) {  
        seaLevelPressure -= 1.0; //Decrease sea level pressure
        isAdjusting = true; 
      } 
      else if (xValue > 3000 && lastJoyX <= 3000) {  
        seaLevelPressure += 1.0; //Increase sea level pressure
        isAdjusting = true; 
      }  
    }
    // Refresh data every second
    if (isAdjusting || (millis() - lastRefresh > 1000)) { 
      drawDataPage(); 
      lastRefresh = millis(); 
    }
  }

  lastJoyY = yValue; //Renew the y value
  lastJoyX = xValue; //Renew the x value
  lastButtonState = btnValue; //Renew the button state
  delay(50); 
}
 