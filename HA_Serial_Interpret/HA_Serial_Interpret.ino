const byte numChars = 32;
char receivedChars[numChars];
boolean NewData = false;

void setup(void)
  {
    // Setup Serial and wait until it is ready.
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Arduino is ready.");
  }

void loop(void)
  {
    ReadData();
    UseData();
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
    if (NewData == true) 
      {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        NewData = false;
      }
  }

