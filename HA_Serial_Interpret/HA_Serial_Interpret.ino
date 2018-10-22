const byte numChars = 32;
char receivedChars[numChars];
boolean NewData = false;

#define LightSensor A0
#define LightRelay 2

#define FanSensor A1
#define FanRelay 3

char* circuit;
char* command;
float current;

void setup()
  {
    // Setup Serial and wait until it is ready.
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Arduino is ready.");
  }

void loop()
  {
    ReadData();
    
    if (NewData == true)
      {
        UseData();
      }
    
    delay(500);
  }
  
//_________________________________________________________________________  
void ReadData()
  {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0 && NewData == false)
      {
        rc = Serial.read();
        
        if (recvInProgress == true)
          {
             if (rc != endMarker)
               {
                 receivedChars[ndx] = rc;
                 ndx++;
                 if (ndx >= numChars) 
                   {
                     ndx = numChars - 1;
                   }
               }
             else
               {
                 receivedChars[ndx] = '\0'; // terminate the string
                 recvInProgress = false;
                 ndx = 0;
                 NewData = true;
               }
          }
        else if (rc == startMarker) 
          {
            recvInProgress = true;
          }
      }
  }
  
//_________________________________________________________________________ 
void UseData() 
  { 
    NewData = false;
    circuit = strtok(receivedChars, ",");
    command = strtok(NULL, ",");
    
    Serial.println("USE DATA");
    Serial.print("Circuit: ");
    Serial.println(circuit);
    Serial.print("Command: ");
    Serial.println(command);
    
    // Tell ReadCurrent() which sensor to read
    if (strcmp(circuit, "Lights") == 0)
      {
        ToggleRelay(LightSensor, LightRelay);
      }
    else if (strcmp(circuit, "Fan") == 0)
      {
        ToggleRelay(FanSensor, FanRelay);
      }
  }

//_________________________________________________________________________ 
void ToggleRelay(const int sensor, const int relay)
  {
    current = ReadCurrent(sensor);
    
    if (strcmp(command, "ON") == 0 && current == 0)
      {
        digitalWrite(relay, !digitalRead(relay));
      }
    else if (strcmp(command, "OFF") == 0 && current != 0)
      {
        digitalWrite(relay, !digitalRead(relay));
      }
  }

//_________________________________________________________________________ 
float ReadCurrent(const int sensor)
  {
    float result;
    int ReadValue;
    int MaxValue = 0;
    unsigned long StartTime = millis();
    
    while ((millis()-StartTime) < 1000)
      {
        ReadValue = analogRead(sensor);
        if (ReadValue > MaxValue) 
          {
            MaxValue = ReadValue;
          }
      }
    
   // Convert the digital data to a voltage
   // result = (maxValue * 5.0)/1024.0;
   
   // Use Ohms law to calculate current across resistor and express in mA 
   // nCurrThruResistorPP = (nVPP/200.0) * 1000.0;
   
   //Use Formula for SINE wave to convert to RMS 
   // nCurrThruResistorRMS = nCurrThruResistorPP * 0.707;
   
   // Current Transformer Ratio is 1000:1...
   // nCurrentThruWire = nCurrThruResistorRMS * 1000;
   
   result = MaxValue * 17.2607;
   
   return result;
  }

