/*
 * NodeMCU as Master
 * Use ArduinoJson.h v5.13.5
 * use ESP8266 board v2.4.1
*/

// ================================
// Libraries
// ================================

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <Wire.h>

// ================================
// Firebase auth
// ================================
#define FIREBASE_HOST "siabrecycle2021-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "JPyoZO2D4zcv2Bzs6Rzy4zLOiyMtWTNK321N4NFr"

// ================================
// Wi-Fi credentials
// ================================
#define WIFI_SSID "Wew"
#define WIFI_PASSWORD "1234567890"

// ================================
// Firebase path
// ================================
String volumeInputPath = "/siabrecycle/volume_input_value";
String volumeOutputPath = "/siabrecycle/volume_output_value";
String tdsInputPath = "/siabrecycle/tds_input_value";
String tdsOutputPath = "/siabrecycle/tds_output_value";
String turbidityInputPath = "/siabrecycle/turb_input_value";
String turbidityOutputPath = "/siabrecycle/turb_output_value";

String phPath = "/siabrecycle/ph_value";
String suhuPath = "/siabrecycle/suhu_value";

String processPath = "/siabrecycle/process_step"; // to indicate step in filtering
String statusPath = "/siabrecycle/status_alat"; // to indicate that device is on or off

// ================================
// Other Variables
// ================================
String temporaryI2CData = "";
String I2CSensorName = "";
float I2CSensorValue;

float phValue;
float temperatureValue;

float volInputValue;
float volOutputValue;
int tdsInputValue;
int tdsOutputValue;
float turbInputValue;
float turbOutputValue;
int processValue;


void setup() 
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Start I2C communication
  Wire.begin(D1, D2);
}

void loop() 
{

  // get data from slave address 8
  Wire.requestFrom(8, 22);
  readI2C();
  // Parsing data
  phValue = parsingData(temporaryI2CData, ',', 0).toFloat();
  tdsOutputValue = parsingData(temporaryI2CData, ',', 1).toInt();
  temperatureValue = parsingData(temporaryI2CData, ',',2).toFloat();
  volOutputValue = parsingData(temporaryI2CData, ',', 3).toFloat();

  //reset temporary i2c
  temporaryI2CData="";
  // Upload to firebase
  Firebase.setFloat(phPath, phValue);
  Firebase.setInt(tdsOutputPath, tdsOutputValue);
  Firebase.setFloat(suhuPath, temperatureValue);
  Firebase.setFloat(volumeOutputPath, volOutputValue);

  // get data from slave address 9
  Wire.requestFrom(9, 26);
  readI2C();
  // Parsing data
  volInputValue = parsingData(temporaryI2CData, ',', 0).toFloat();
  turbInputValue = parsingData(temporaryI2CData, ',', 2).toFloat();
  turbOutputValue = parsingData(temporaryI2CData, ',', 3).toFloat();
  processValue = parsingData(temporaryI2CData, ',', 4).toInt();
  //reset temporary i2c
  temporaryI2CData="";
  // Upload to firebase
  Firebase.setFloat(volumeInputPath, volInputValue);
  Firebase.setFloat(turbidityInputPath, turbInputValue);
  Firebase.setFloat(turbidityOutputPath, turbOutputValue);
  Firebase.setInt(processPath, processValue);
  
  delay(1000);
}

void readI2C()
{
  while(Wire.available())
  {
    char c = Wire.read();
    temporaryI2CData = temporaryI2CData + c;
  }
  Serial.println(temporaryI2CData);

}

// parsing mechanism
String parsingData(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

