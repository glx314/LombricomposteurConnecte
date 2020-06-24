const int pHSensorPin = A0 ; //réglages pH
float pHOffset=0.00;
const int samplingInterval = 20;
const int printInterval = 800;
const int ArrayLenth = 40 ;
int pHArray[ArrayLenth];
int pHArrayIndex=0;
int humidity=0;
const int testpHButtonPin = 8;
const int testpHLEDPin = 7;
bool testpH=false;
const int tempsTestPH = 5000;

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

void setup() {
  // put your setup code here, to run once:
 pinMode(testpHButtonPin,INPUT_PULLUP);
 pinMode(testpHLEDPin,OUTPUT); 
}

void loop() {
   testpH=not(digitalRead(testpHButtonPin));
   digitalWrite(testpHLEDPin,LOW);// put your main code here, to run repeatedly:
if (testpH){ //Mesure du pH 
    digitalWrite(testpHLEDPin,HIGH);
    int compteurPH=0;
    while(compteurPH<tempsTestPH){
    digitalWrite(testpHLEDPin,HIGH);
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
    
    
    Serial.print("pH = ");
    Serial.println(pHValue);
    
        
        printTime=millis();
  }

    compteurPH+=100;    
    delay(100);
    }
}
}
