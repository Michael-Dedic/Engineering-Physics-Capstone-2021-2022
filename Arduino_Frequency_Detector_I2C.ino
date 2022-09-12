#include "arduinoFFT.h"         //Need to use this library in order to perform the FFT
//#include <Servo.h>              // Need this to setup servo
#define SAMPLES 512    //Samples for the FFT. The number has to be in base 2 and the highest for the Uno is 128
#define SAMPLING_FREQUENCY 2048 //Sampling frequency can be changed, however, it must follow the nyuist rule
//----------------------------------------------------------------------------------------------------------------------
#include <Wire.h>
#define I2C_SLAVE_ADDRESS 8 
#define PAYLOAD_SIZE 2
int State= 1;

//----------------------------------------------------------------------------------------------------------------------



arduinoFFT FFT = arduinoFFT();        
void(* resetFunc) (void) = 0; // create a standard reset function 
unsigned int samplingPeriod; // create a sampling period variable
unsigned long microSeconds;  //create a variable to determine how long the began running the script in microseconds
double vReal[SAMPLES]; //create vector of size SAMPLES to hold real values
double vImag[SAMPLES]; //create vector of size SAMPLES to hold imaginary values
double StorageArray[SAMPLES]; //Storing values in Array for peak frequencies
double Frequency; //create a variable that will be used later on in the code
double peak;
double Newpeak;


//---------------------------------------------------------------------------

#define spokeSamples 2
double freqValues[spokeSamples];
double ampValues[spokeSamples];
int maxIndex = 0;
double maxValue;
volatile float freqMeasurement=0;
double analogMeasurement=0;
int analogIndex=0;

//---------------------------------------------------------------------------
//Servo section-------------------------------------------------------------
//Servo myservo;  //Need to create object to control servo
//--------------------------------------------------------------------------


 
void setup() 
{
    //Serial.begin(9600); //Baud rate for the Serial Monitor
    Serial.begin(115200);
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY)); //Period in microseconds 
    //myservo.attach(9,600,2300); // (pin, min, max) setting up the servo corresponding to what values give min and max angles. Uncomment when using servo.
    
    Wire.begin(I2C_SLAVE_ADDRESS); // Initializes the connection for I2C
    delay(1000);               
    Wire.onRequest(requestEvents); // Calls requestevents when given instructions from the master
    Wire.onReceive(receiveEvents); //Calls recieveevents when given instructions from the master
    
}

void receiveEvents(int numBytes) //recieving data
{  
  Serial.println(F("---> recieved events"));
  //freqMeasurement=0;
  State= Wire.read();      // Used as an interrupt to start the FFT
  Serial.println(State);
  delay(100);
  //noInterrupts();
  // freqMeasurement=0; 
  //interrupts();

  
     
}

 
void loop() 
{  

    if(State ==1){
    Serial.println('starting measurement');
    Serial.println("---------------------");
    for( int j=0; j<spokeSamples; j++)
    {

    for(int i=0; i<SAMPLES; i++)
    {
        
        
         microSeconds = micros();    //Returns the number of microseconds since the Arduino board began running the current script. 
         vReal[i] = analogRead(0); //Reads the analog input from the Arduino and saves it as a real value
         vImag[i] = 0; //Imaginary values will be 0 for now, but it will be needed later for the FFT
    
   
    
        while(micros() < (microSeconds + samplingPeriod))
        {
          // wait until we can sample again
        }
    }
    
     for(int k = 1; k < SAMPLES; k++) //Finding Max of analog values
    {
    if(vReal[k] > maxValue) {
        maxValue = vReal[k];
        maxIndex = k;
    }
      }
    
    //Next 3 lines peform the FFT
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY); // Finds the frequecy with the highest amplitude
    double Newpeak= peak*0.976259; //Multplies by the scaling factor

   
    freqValues[j]= Newpeak;
    ampValues [j]= maxValue;
    
    


    
    
    //Serial.println(Newpeak);
    //Serial.println('x');
    //Serial.println(maxValue);
    
    
    } 
     for(int z = 1; z < spokeSamples; z++) //Finding Max of analog values
    {
    if(ampValues[z] > analogMeasurement ) {
        analogMeasurement = ampValues[z];
        analogIndex = z;
    }
      }
   noInterrupts();
   freqMeasurement= freqValues[analogIndex];
   Serial.println("sampling complete");
   Serial.println(String(freqMeasurement) + String("Hz"));
   Serial.println(String(analogMeasurement)  + String(" mV"));
   
    State=0;
   interrupts();
   

      
     
   //resetFunc();
    }
    else{
      delay(100);
    }   
}
//--------------------------------------------------------------------------------------------------------



void requestEvents()  //Sends data
{
  
  Serial.println(F("---> recieved request"));
  
  
  Wire.write((uint8_t *) &freqMeasurement, sizeof freqMeasurement);
  Serial.println("Message Sent");
  Serial.println(String(freqMeasurement)+ String(" Hz")); 
  
}
