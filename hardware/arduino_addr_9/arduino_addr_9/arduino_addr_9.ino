#include <Wire.h>

String I2CMessage = "";

float turbInputValue=0.00;
float turbOutputValue=0.00;
float volInputValue=0.00;
float volOutputValue=0.00;
int processValue=0;

// processValue 0 : set when no water in tank. (before filtration process)
// processValue 1 : Filtration Process
// processValue 2 : set when water is ready. (filtration process done)

void setup() {
 Wire.begin(9);  // I2C address 9
 Wire.onRequest(requestEvent); /* register request event */
}

void loop() {
  //==========================
  // Code
  //==========================



  //==========================
  // Update I2C Message
  //==========================
  I2CMessage = String(volInputValue, 2) + "," + String(volOutputValue, 2) + "," + String(turbInputValue, 2) + "," + String(turbOutputValue, 2) + "," + String(processValue, 0) + ",";
  Serial.println(I2CMessage);
}

void requestEvent() {
  // Convert to string
  char buffer[26];
  I2CMessage.toCharArray(buffer, 26);
  // Send message when master need
  Wire.write(buffer);
}
