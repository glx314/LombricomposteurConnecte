#include <TheThingsNetwork.h>
#include <DS18B20.h>
// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "97a685c64a870a4f72db1015b81dcaf3";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
DS18B20 ds();

#define RL 47  //The value of resistor RL is 47K
#define m -0.263 //Enter calculated Slope
#define b 0.42 //Enter calculated intercept
#define Ro 30 //Enter found Ro value
#define TempPin A5
#define HumPin A3
#define pHPin A0
#define buttonPin 3

int buf[10],temp;


void setup()
{
  
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  pinMode(TempPin,INPUT);
  pinMode(pHPin,INPUT);
  pinMode(HumPin,INPUT);
  pinMode(buttonPin, INPUT);
  ds.begin();
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
  int h = analogRead(HumPin);//on lit l'hygrometrie
  uint32_t t = ds.getTempC(TempPin);
  float analog_value;
  for(int test_cycle = 1 ; test_cycle <= 500 ; test_cycle++) //Read the analog output of the sensor for 200 times
  {
    analog_value = analog_value + analogRead(A0); //add the values for 200
  }
  analog_value = analog_value/500.0; //Take average
  float VRL = analog_value*(5.0/1023.0); //Convert analog value to voltage

  float Rs = ((5.0/VRL)-1) * RL;
  float ratio = Rs/Ro; 
  double ppm = pow(10, ((log10(ratio)-b)/m));
  
   
   h = map(h,0,1023,0,100);
   uint32_t hEncode = h;
   uint32_t ppmEncode = ppm;
   
  if (isnan(h) || isnan(t)|| isnan(ppm))
 {
   Serial.println("Failed to read from sensor!");
   return;
 }

    
    byte payload[6];
    payload[0] = highByte(hEncode);
    payload[1] = lowByte(hEncode);
    payload[2] = highByte(t);
    payload[3] = lowByte(t);
    payload[4] = highByte(ppmEncode);
    payload[5] = lowByte(ppmEncode);
  ttn.sendBytes(payload,sizeof(payload));



  if(digitalRead(buttonPin)){

    for(int i=0;i<10;i++){
      buf[i]=analogRead(pHPin);
      delay(10);
      }
    for(int i=0;i<9;i++){
        for(int j=i+1;j<10;j++){
          if(buf[i]>buf[j]){
            temp=buf[i];
            buf[i]=buf[j];
            buf[j]=temp;
            }
          }
    }
    float avgValue = 0;
    for(int i=2;i<8;i++){
        avgValue+=buf[i];
      } 
    float pHValue=avgValue*5.0/1024/6;
    pHValue = 3.5*pHValue; 
    uint32_t pHEncode = pHValue;
      
    byte payloadPH[2];
    payload[0]=highByte(pHEncode);
    payload[1]=lowByte(pHEncode);

    ttn.sendBytes(payloadPH,sizeof(payloadPH));
    }
    
  delay(10000);
}
