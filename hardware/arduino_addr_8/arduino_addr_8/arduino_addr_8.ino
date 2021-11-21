/*
* TDS Reading Reference: DFRobot
*/

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 9
#define phPin A0
#define TdsSensorPin A1

#define VREF 5.0 
#define SCOUNT  30

// Temperature Sensor

OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature DS18B20Sensor(&oneWire);

float DS18B20Temperature = 0.00;

// pH Sensor
float phTemporary = 0.00;
float phAnalog = 0;
float phValue = 0.00;
float phCalibration = 21.80;

int readingInterval = 1000;

// TDS Sensor
int analogBuffer[SCOUNT];   
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0, temperature = 25;

float TDSValue=0;

// Out Volume Sensor
const int trigPin = 6;
const int echoPin = 10;
long duration;
int distance;
int t_bottom = 24; //cm
float volOutputValue=0.00;

// Other(s)
String I2CMessage = "";


void setup() {
 Wire.begin(8);  // I2C adress
 Wire.onRequest(requestEvent); /* register request event */
 Serial.begin(9600);           /* start serial for debug */

 DS18B20Sensor.begin();

 pinMode(phPin, INPUT);
 pinMode(TdsSensorPin,INPUT);

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
}

void loop() {
  
  static unsigned long readingTime = millis();

  if (millis() - readingTime > readingInterval)
  {
    readingTime = millis();

    //========================================
    // Read Output Volume
    //========================================
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // V = p x l x t
    volOutputValue = 18.0 * 18.0 * (t_bottom - distance) / 1000.0;
    
    //========================================
    // Read water temperature
    //========================================
    DS18B20Sensor.requestTemperatures();
    DS18B20Temperature = DS18B20Sensor.getTempCByIndex(0);

    if (DS18B20Temperature < 0)
    {
      DS18B20Temperature = 27.15;
    }

    //========================================
    // Read water pH
    //========================================

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


    //========================================
    // Read TDS after filtration
    //========================================
    static unsigned long analogSampleTimepoint = millis();
    if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
    {
      analogSampleTimepoint = millis();
      analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
      analogBufferIndex++;
      if(analogBufferIndex == SCOUNT) 
          analogBufferIndex = 0;
    }   
    static unsigned long printTimepoint = millis();
    if(millis()-printTimepoint > 800U)
    {
        printTimepoint = millis();
        for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
          analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
        averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0;
        float compensationCoefficient=1.0+0.02*(temperature-25.0); 
        float compensationVolatge=averageVoltage/compensationCoefficient; 
        TDSValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;
    }

    if(TDSValue < 0)
    {
      TDSValue = 0;
    }

    if (TDSValue > 2000)
    {
      TDSValue = 2000;
    }

    //========================================
    // I2C Message
    //========================================
    I2CMessage = String(phValue, 2) + "," + String(TDSValue, 0) + "," + String(DS18B20Temperature, 2) + "," + String(volOutputValue, 2) + ",";
    Serial.println(I2CMessage);

  }
}

void requestEvent() {
  // Convert to string
  char buffer[22];
  I2CMessage.toCharArray(buffer, 22);
  // Send message when master need
  Wire.write(buffer);
}

int getMedianNum(int bArray[], int iFilterLen) 
{
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) 
  {
  for (i = 0; i < iFilterLen - j - 1; i++) 
      {
    if (bTab[i] > bTab[i + 1]) 
        {
    bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
    bTab[i + 1] = bTemp;
      }
  }
  }
  if ((iFilterLen & 1) > 0)
bTemp = bTab[(iFilterLen - 1) / 2];
  else
bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}