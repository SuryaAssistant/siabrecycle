#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 9
#define phPin A0

// Create one wire
OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature DS18B20Sensor(&oneWire);

float DS18B20Temperature = 0.00;

float phTemporary = 0.00;
float phAnalog = 0;
float phValue = 0.00;
float phCalibration = 25.20;

int readingInterval = 1000;

String I2CMessage = "";

void setup() {
 Wire.begin(8);  // I2C adress
 Wire.onRequest(requestEvent); /* register request event */
 Serial.begin(9600);           /* start serial for debug */

 DS18B20Sensor.begin();

 pinMode(A0, INPUT);
}

void loop() {
  
  static unsigned long readingTime = millis();

  if (millis() - readingTime > readingInterval)
  {
    readingTime = millis();

    // request tempereature
    DS18B20Sensor.requestTemperatures();
    DS18B20Temperature = DS18B20Sensor.getTempCByIndex(0);

    if (DS18B20Temperature < 0)
    {
      DS18B20Temperature = 27.15;
    }

    // Read pH
    for (int i=0; i<100; i++)
    {
      phTemporary = phTemporary + analogRead(phPin);
    }

    phAnalog = phTemporary / 100.0;

    //reset value
    phTemporary = 0.00;

    phValue = -5.70 * (phAnalog * 5.0 / 1024.0) + phCalibration;

    if (phValue < 0)
    {
      phValue = 0.00;
    }

    //update message
    I2CMessage = String(phValue, 2) + "," + String(DS18B20Temperature, 2) + ",";
    Serial.println(I2CMessage);

  }


}

void requestEvent() {
  // Convert to string
  char buffer[11];
  I2CMessage.toCharArray(buffer, 11);
  // Send message when master need
  Wire.write(buffer);
}