#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// Define default I2C pins for ESP32-S3-DevKitC-1
#define I2C_SDA 8
#define I2C_SCL 9

// Create an instance of the BMP280 library
Adafruit_BMP280 bmp;

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for native USB serial to initialize
  }

  Serial.println(F("BMP280 Sensor Initialization..."));

  // Initialize custom I2C pins for the ESP32-S3
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the BMP280 sensor explicitly at address 0x76
  unsigned status = bmp.begin(0x76);
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
}

void loop() {
  // Read temperature in Celsius
  float temperature = bmp.readTemperature();
  
  // Read pressure in Pascals and convert to hPa (hectopascals/millibars)
  float pressure = bmp.readPressure() / 100.0F;
  
  // Calculate approximate altitude based on standard barometric pressure (1013.25 hPa)
  float altitude = bmp.readAltitude(1024.0);

  // Print results to Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °C"));

  Serial.print(F("Pressure:    "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));

  Serial.print(F("Approx. Alt: "));
  Serial.print(altitude);
  Serial.println(F(" m"));

  Serial.println(F("-----------------------------------"));
  
  delay(2000); // Wait 2 seconds between readings
}