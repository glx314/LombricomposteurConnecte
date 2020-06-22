#include <DS18B20.h>
#include <LiquidCrystal.h>

LiquidCrystal screen(12,11,A2,A3,A4,A5);

const int DS18B20_PIN=2;
DS18B20 ds(DS18B20_PIN);
float DS18B20_temperature; //réglages température

const int humiditySensorPin=A0; //réglages humidité
int humidity=0;

const int pHSensorPin = A1 ; //réglages pH
float pHOffset=0.00;
const int samplingInterval = 20;
const int printInterval = 800;
const int ArrayLenth = 40 ;
int pHArray[ArrayLenth];
int pHArrayIndex=0;
const int tempsTestPH = 60000;

const int maintenanceButtonPin = 10; //gestion des boutons
const int maintenanceLEDPin=9;
bool maintenance=false;  
const int TempsMaintenance = 15;

const int testpHButtonPin = 8;
const int testpHLEDPin = 7;
bool testpH=false;

const int calibNeutreButtonPin=6;
const int calibNeutreLEDPin = 5;
bool calibNeutre=false;

const int calibAcidButtonPin=4;
const int calibAcidLEDPin=3;
bool calibAcid=false;
int tempsCalibPH = 60000;
int compteur=0;

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

void setup()
{
  Serial.begin(9600);
  screen.begin(16,2);
  screen.clear();
  pinMode(maintenanceButtonPin, INPUT_PULLUP);
  pinMode(testpHButtonPin,INPUT_PULLUP);
  pinMode(calibNeutreButtonPin,INPUT_PULLUP);
  pinMode(calibAcidButtonPin,INPUT_PULLUP);
  pinMode(maintenanceLEDPin,OUTPUT);
  pinMode(testpHLEDPin,OUTPUT);
  pinMode(calibNeutreLEDPin,OUTPUT);
  pinMode(calibAcidLEDPin,OUTPUT);
  pinMode(DS18B20_PIN,INPUT);
  pinMode(humiditySensorPin,INPUT);
  
}

void loop()
{
  digitalWrite(maintenanceLEDPin,HIGH); //Activation des leds
  digitalWrite(testpHLEDPin,HIGH);
  digitalWrite(calibNeutreLEDPin,HIGH);
  digitalWrite(calibAcidLEDPin,HIGH);

  maintenance = not(digitalRead(maintenanceButtonPin));
  testpH=not(digitalRead(testpHButtonPin));
  calibNeutre=not(digitalRead(calibNeutreButtonPin));
  calibAcid=not(digitalRead(calibAcidButtonPin));

  if(compteur==3000){
  DS18B20_temperature = ds.getTempC(); //Mesure température
  Serial.println(DS18B20_temperature);
  
  humidity=analogRead(humiditySensorPin);
  Serial.println(humidity); // ATTENTION : TESTER LES VALEURS POUR MAPPER EN RECEVANT LE CAPTEUR

  screen.clear();
  screen.setCursor(0,0);
  screen.print("Temperature :");
  screen.print(DS18B20_temperature);
  screen.println("C");
  screen.setCursor(0,1);
  screen.print("Humidité :");
  screen.print(humidity);
  screen.print("%");

  }
  
  if (maintenance){ //Maintenance : on ne prend plus de mesures
    digitalWrite(maintenanceLEDPin,LOW);
    int minute=TempsMaintenance;
    int seconde=0;
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
      delay(1000);
      seconde--;
    }
    
  }
   
  
  if (testpH){ //Mesure du pH 
    digitalWrite(testpHLEDPin,LOW);
    int compteurPH=0;
    while(compteurPH<tempsTestPH){
    
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue,voltage;
  
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+pHOffset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
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
  
  if (calibNeutre){ // Calibration sonde pH dans solution neutre
    digitalWrite(calibNeutreLEDPin,LOW);
    static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+pHOffset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    
        pHOffset=7-pHValue;
        screen.clear();
         screen.setCursor(0,0);
         screen.print("pH = ");
         screen.print(pHValue);
         printTime=millis();
  }
    delay(3000);
    
  }
  
  if (calibAcid){ // Calibration sonde pH dans solution acide
    digitalWrite(calibAcidLEDPin,LOW);
    int compteurPHA=0;
    while(compteurPHA<tempsCalibPH){
    static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(pHSensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+pHOffset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    screen.clear();
    screen.setCursor(0,0);
    screen.print("pH = ");
    screen.print(pHValue);
    printTime=millis();
  }

    compteurPHA+=100; 
    delay(100);
    }
  }
   
  compteur+=100;
  if(compteur>3000){
      compteur=0;
  }
  delay(100); 
}
