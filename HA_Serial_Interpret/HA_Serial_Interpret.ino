const byte numChars = 32;
char receivedChars[numChars];
boolean NewData = false;

#define LightRelay = A0;
#define FanRelay = A1;

char* circuit;
char* command;

void setup(void)
  {
    // Setup Serial and wait until it is ready.
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Arduino is ready.");
  }

void loop(void)
  {
    ReadData(circuit,command);
    
    if (NewData == true)
      {
        UseData(circuit, command);
      }
  }
  
//_________________________________________________________________________  
void ReadData(char* CircuitOut,char* CommandOut)
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
    
    CircuitOut = strtok(receivedChars, ",");
    CommandOut = strtok(NULL, ",");
    return;
  }
  
//_________________________________________________________________________ 
char UseData(char* CircuitIn,char* CommandIn) 
  {
    char CircuitChar = char(CircuitIn);
    char CommandChar = char(CommandIn);
    
    NewData = false;
    
    Serial.println("USE DATA");
    Serial.print("Circuit: ");
    Serial.println(CircuitChar);
    Serial.print("Command: ");
    Serial.println(CommandChar);
    
    switch (CircuitChar)
      {
        case 'Lights':
          Serial.println("CHANGING LIGHTS");
          break;
        case 'Fan':
          Serial.println("CHANGING FAN");
          break;
      }
  }

