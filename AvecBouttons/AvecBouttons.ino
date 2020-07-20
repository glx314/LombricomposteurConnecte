#include <TheThingsNetwork.h>
#include <LoraEncoder.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
/********************************************************************/

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "97a685c64a870a4f72db1015b81dcaf3";
#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

LiquidCrystal screen(12,11,A2,A3,A4,A5);

const int TempsReleve = 10000;

float DS18B20_temperature; //réglages température

const int humiditySensorPin=A0; //réglages humidité
int humidity;
int pHumidity;

const int pHSensorPin = A1 ; //réglages pH
const int samplingInterval = 20;
const int printInterval = 800;
const int ArrayLenth = 40 ;
int pHArray[ArrayLenth];
int pHArrayIndex=0;
const int tempsTestPH = 30000; //RISQUE D ETRE TROP GRAND, PENSER A CHANGER LE TYPE

const int maintenanceButtonPin = 10; //gestion des boutons
const int maintenanceLEDPin=9;
bool maintenance=false;  
const int TempsMaintenance = 1;

const int testpHButtonPin = 8;
const int testpHLEDPin = 7;
bool testpH=false;


const int calibNeutreButtonPin=6;
const int calibNeutreLEDPin = 5;
bool calibNeutre=false;


const int calibAcidButtonPin=4;
const int calibAcidLEDPin=3;
bool calibAcid=false;
int tempsCalibPH = 30000;
int compteur=0;
float pente = 3.5;
float Offset = 0;
float VA;
float V0;

void setup(){
  
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  screen.begin(16,2);
  screen.clear();
  sensors.begin();
  
  pinMode(maintenanceButtonPin, INPUT_PULLUP);
  pinMode(testpHButtonPin,INPUT_PULLUP);
  pinMode(calibNeutreButtonPin,INPUT_PULLUP);
  pinMode(calibAcidButtonPin,INPUT_PULLUP);
  pinMode(maintenanceLEDPin,OUTPUT);
  pinMode(testpHLEDPin,OUTPUT);
  pinMode(calibNeutreLEDPin,OUTPUT);
  pinMode(calibAcidLEDPin,OUTPUT);
  pinMode(ONE_WIRE_BUS,INPUT);
  pinMode(humiditySensorPin,INPUT);
  
   // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
  
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  
}

void loop(){
  
  digitalWrite(maintenanceLEDPin,LOW); //Activation des leds
  digitalWrite(testpHLEDPin,LOW);
  digitalWrite(calibNeutreLEDPin,LOW);
  digitalWrite(calibAcidLEDPin,LOW);

  maintenance = not(digitalRead(maintenanceButtonPin));
  testpH=not(digitalRead(testpHButtonPin));
  calibNeutre=not(digitalRead(calibNeutreButtonPin));
  calibAcid=not(digitalRead(calibAcidButtonPin));
  sensors.requestTemperatures();
  DS18B20_temperature = sensors.getTempCByIndex(0);
  humidity=analogRead(humiditySensorPin);
  
  if(compteur==TempsReleve){
    debugSerial.println("-- LOOP");
     //Mesure température
    Serial.println(DS18B20_temperature);
  
    
    pHumidity=map(humidity,0,605,0,100);
    Serial.println(pHumidity); // ATTENTION : TESTER LES VALEURS POUR MAPPER EN RECEVANT LE CAPTEUR

    screen.clear();
    screen.setCursor(0,0);
    screen.print("Temp : ");
    screen.print(DS18B20_temperature);
    screen.print(" ");
    screen.print((char)223);
    screen.println("C     ");
    screen.setCursor(0,1);
    screen.print("Humidite : ");
    screen.print(pHumidity);
    screen.print(" %");

    byte TPayload[2];
    int sizeT=sizeof(TPayload);
    byte HPayload[1];
    int sizeH=sizeof(HPayload);
    int T=round(DS18B20_temperature*10.0);
    TPayload[0]=highByte(T);
    TPayload[1]=lowByte(T);
    byte H;
    H=(int)pHumidity;
    HPayload[0]=H;

    byte payload[sizeT + sizeH];
    memcpy(payload, TPayload, sizeT);
    memcpy(payload + sizeT, HPayload, sizeH);
    
    
    ttn.sendBytes(payload,sizeof(payload));
    for(int i=0;i<sizeof(payload);i++){
      Serial.print(payload[i]);
      Serial.print(";");
    }
    Serial.println("");

  }
  
  if (maintenance){fMaintenance();}
  
  if (testpH){fTestpH();}
  
  if (calibNeutre){fCalibNeutre();}
  
  if (calibAcid){ fCalibAcide();}
   
  compteur+=5000;
  if(compteur>TempsReleve){
      compteur=0;
  }
  
  delay(5000); 
  
}


void fMaintenance(){
       //Maintenance : on ne prend plus de mesures
    digitalWrite(maintenanceLEDPin,HIGH);
    int minute=TempsMaintenance;
    int seconde=10;
    while(seconde != 0 || minute != 0){
    screen.clear();
    screen.setCursor(0,0);
    screen.print("Maintenance");
    screen.setCursor(0,1);
    screen.print("Patientez  ");  
    screen.print(minute);
    screen.print(":");
      
    if(seconde<10){screen.print("0");}
    
    screen.print(seconde);
    
    if(seconde == 0 && minute!=0){
        minute--;
        seconde=60;
      }

    for(int i=0;i<10;i++){
        delay(100);
        maintenance = not(digitalRead(maintenanceButtonPin));
        if(maintenance){
          minute=TempsMaintenance;
          seconde=60;
          }
        }
      
    seconde--;
      }
      
   }

void fTestpH(){//Mesure du pH 
  
    digitalWrite(testpHLEDPin,HIGH);
    int compteurPH=0;
    while(compteurPH<tempsTestPH){
      static unsigned long samplingTime = millis();
      static unsigned long printTime = millis();
      static float pHValue,voltage;
  
      if(millis()-samplingTime > samplingInterval){
          pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
          if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
          voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
          pHValue = pente*voltage+Offset;
          samplingTime=millis();
              }
      if(millis() - printTime > printInterval){   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  
          screen.clear();
          screen.setCursor(0,0);
          screen.print("pH = ");
          screen.print(pHValue);
          printTime=millis();
              }

      compteurPH+=100;

    
      delay(100);
       
       }
   }

void fCalibNeutre(){
     // Calibration sonde pH dans solution neutre
    digitalWrite(calibNeutreLEDPin,HIGH);
    int compteurPHN=0;
    while(compteurPHN<tempsCalibPH){
        static unsigned long samplingTime = millis();
        static unsigned long printTime = millis();
        static float pHValue,voltage;
        if(millis()-samplingTime > samplingInterval){
            pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
            if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
            V0 = avergearray(pHArray, ArrayLenth)*5.0/1024;
            voltage=V0;
            pHValue = pente*voltage + Offset;
            samplingTime=millis();
            }
            
        if(millis() - printTime > printInterval){   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  
            screen.clear();
            screen.setCursor(0,0);
            screen.print("pH = ");
            screen.print(pHValue);
            printTime=millis();
             }

        compteurPHN+=100; 
        delay(100);
        }

  pente=3/(V0-VA);
  Offset = 4-(3*VA)/(V0-VA);
    
  }

 void fCalibAcide(){
    // Calibration sonde pH dans solution acide
    digitalWrite(calibAcidLEDPin,HIGH);
    int compteurPHA=0;
    while(compteurPHA<tempsCalibPH){
        static unsigned long samplingTime = millis();
        static unsigned long printTime = millis();
        static float pHValue,voltage;
  
        if(millis()-samplingTime > samplingInterval){
            pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
            if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
            VA = avergearray(pHArray, ArrayLenth)*5.0/1024;
            voltage=VA;
            pHValue = pente*voltage+Offset;
            samplingTime=millis();
            }
            
        if(millis() - printTime > printInterval){   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  
            screen.clear();
            screen.setCursor(0,0);
            screen.print("pH = ");
            screen.print(pHValue);
            printTime=millis();
             }

        compteurPHA+=100; 
        delay(100);
           }
    
    pente=3/(V0-VA);
    Offset = 4 -(3*VA)/(V0-VA) ;
    }  

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }
  else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
  }
