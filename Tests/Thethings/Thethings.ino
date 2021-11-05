#include <TheThingsNetwork.h>
#include <LoraEncoder.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "97a685c64a870a4f72db1015b81dcaf3";
const int humiditySensorPin=A0; //réglages humidité

#define loraSerial Serial1
#define debugSerial Serial

int humidity;
int pHumidity;
float DS18B20_temperature;
// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

 
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();
  
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.println("-- LOOP");
  sensors.requestTemperatures();
  
  DS18B20_temperature = sensors.getTempCByIndex(0);

    humidity=analogRead(humiditySensorPin);
    pHumidity=map(humidity,0,605,0,100);//on lit l'hygrometrie

    byte buffer[4];
    LoraEncoder encoder(buffer);
    encoder.writeTemperature(DS18B20_temperature);
    encoder.writeHumidity(pHumidity);
    
  ttn.sendBytes(buffer,sizeof(buffer));

  delay(10000);
}
