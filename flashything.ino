
// sensorPin == input from comparator
int sensorPin = A0;

// pin for IR diode
int IRled = 7;

// internal LED
int ledPin = 13;

// pin with optocouplers, for triggering flashes/cameras
int trigger = 2;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(trigger, OUTPUT);
  pinMode(IRled, OUTPUT);
  
}


void loop() {
  
  if (analogRead(sensorPin) > 600){
    
    digitalWrite(trigger, HIGH);
    shutter();
    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(trigger, LOW);
    digitalWrite(ledPin, LOW);
    Serial.println("Triggered");
    delay(1000);
    
  }
}

// https://playground.arduino.cc/Main/SonyShutterIR/



void shutter()
{
  // Code to send Shutter release command B4B8F
  const static int SCodeBits[] = {1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};

  // Code to send 2 X delay Shutter release command ECB8F (Not used)
  const static int S2CodeBits[] = {1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1};

  for (int i=1; i <= 1; i++)  // Send Command 3 times as per Sony Specs
  {
    header();                    // Send the Start header
    for (int i=0; i <= 19; i++)  // Loop to send the bits
    {
          if(SCodeBits[i] == 1)  // Is Data_is_One to be sent ?
          {
            Data_is_One();              // Yes, send a Data_is_One bit
          }
          else                  // No, Data_is_Zero to be sent
          {
            Data_is_Zero();              // Send a Data_is_Zero bit
          }
    }
    delay(11);                  // Delay Padding to give approx 45mS between command starts
  }
}
// Routine to give the 40kHz burst signal
void burst()                   // 40KHz burst
{
  digitalWrite(IRled, HIGH);   // sets the pin on
  delayMicroseconds(10);       // pauses for 13 microseconds  (fudged to 10uS Delay)   
  digitalWrite(IRled, LOW);    // sets the pin off
  delayMicroseconds(8);        // pauses for 12 microseconds   (fudged to 8uS Delay)
}

// Routine to give a quiet period of data
void quiet()                   // Quiet burst
{
  digitalWrite(IRled, LOW);    // sets the pin off
  delayMicroseconds(10);       // pauses for 13 microseconds   (fudged to 10uS Delay)  
  digitalWrite(IRled, LOW);    // sets the pin off
  delayMicroseconds(8);        // pauses for 12 microseconds    (fudged to 8uS Delay)
}

// Routine to send header data burst
// This allows the IR receiver to set its AGC (Gain)
// Header Burst Timing is 96 * 0.025uS = 2.4mS
// Quiet Timing is 24 * 0.025uS = 600uS
void header() 
{
    for (int i=1; i <= 96; i++){
      burst();                // 40kHz burst
    }
    for (int i=1; i <= 24; i++){
      quiet();                // No 40 kHz
    }
}

// Routine to send one data burst
// Burst Timing is 48 * 0.025uS = 1.2mS
// Quiet Timing is 24 * 0.025uS = 600uS
void Data_is_One()
{
    for (int i=1; i <= 48; i++){
      burst();                // 40kHz burst
    }
    for (int i=1; i <= 24; i++){
      quiet();                // No 40 kHz
    }
}

// Routine to send zero data burst
// Burst Timing is 24 * 0.025uS = 600uS
// Quiet Timing is 24 * 0.025uS = 600uS
void Data_is_Zero()
{
    for (int i=1; i <= 24; i++){
      burst();                // 40 kHz burst
    }
    for (int i=1; i <= 24; i++){
      quiet();                // No 40 kHz 
    }
}
