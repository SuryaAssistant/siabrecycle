#include <Wire.h>

void setup() {
 Wire.begin(8);  // I2C adress
 Wire.onRequest(requestEvent); /* register request event */
 Serial.begin(9600);           /* start serial for debug */
}

void loop() {
 delay(100);
}

void requestEvent() {
  // send string
 Wire.write("ph,07.21,suhu,27.04");
}