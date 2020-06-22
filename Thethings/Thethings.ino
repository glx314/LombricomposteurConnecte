#include <TheThingsNetwork.h>
#include "DHT.h"
// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "97a685c64a870a4f72db1015b81dcaf3";
#define DHTTYPE DHT22
#define DHTPIN A4
#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
DHT dht(DHTPIN,DHTTYPE);
 
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  dht.begin();
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
  uint32_t h = dht.readHumidity();//on lit l'hygrometrie
  uint32_t t = dht.readTemperature();
  if (isnan(h) || isnan(t))
 {
   Serial.println("Failed to read from DHT sensor!");
   return;
 }

    byte payload[4];
    payload[0]=highByte(h);
    payload[1]= lowByte(h);
    payload[2] = highByte(t);
    payload[3] = highByte(t);
  ttn.sendBytes(payload,sizeof(payload));

  delay(10000);
}
