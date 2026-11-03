#include <Arduino.h> 
#include <Wire.h> 
 
//Pin definitions for the joystick
#define JOY_X 15 
#define JOY_Y 16 
#define JOY_SW 17 
 
void setup(void) { 
  Serial.begin(115200);
  Serial.println("Testing Joystick");

  pinMode(JOY_SW, INPUT_PULLUP); // Set the joystick switch pin as input with pull-up resistor

} 

void loop() { 
  int yValue = analogRead(JOY_Y); // Read the Y-axis value of the joystick
  int xValue = analogRead(JOY_X); // Read the X-axis value of the joystick
  int swValue = digitalRead(JOY_SW); // Read the button state of the joystick

  Serial.print("X: ");
  Serial.print(xValue); // Print the X-axis value

  Serial.print(" | Y: ");
  Serial.print(yValue); // Print the Y-axis value

  Serial.print(" | SW: ");
  if (swValue == LOW) { // Check if the button is pressed
    Serial.println(" Pressed"); // Print "Pressed" if the button is pressed
  } else {
    Serial.println(" Released"); // Print "Released" if the button is not pressed
  }
    
  delay(500); // Wait for 500 milliseconds before the next reading
}
 