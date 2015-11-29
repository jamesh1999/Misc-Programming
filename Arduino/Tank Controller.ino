int motorRCPin = 5;
int motorRCAPin = 6;
int motorLCPin = 9;
int motorLCAPin = 10;

int power = 0;
int dir = 0;

int motorR = 0;
int motorL = 0;

String inputString = "";
boolean stringComplete = false;

void setup()
{
  pinMode(motorRCPin, OUTPUT);
  pinMode(motorRCAPin, OUTPUT);
  pinMode(motorLCPin, OUTPUT);
  pinMode(motorLCAPin, OUTPUT);
}

void loop()
{
  if(stringComplete)
  {
     if(inputString[0]=='s')
     {
       power = inputString.substring(1).toInt() - 255;
     } 
     else if(inputString[0]=='r')
     {
       dir = inputString.substring(1).toInt() - 255;
     } 
     
     motorR = power;
     motorL = power;
     
     motorR += dir;
     motorL -= dir;
     
     analogWrite(motorRCPin, min(255, max(0, motorR)));
     analogWrite(motorRCAPin, min(255, max(0, -motorR)));
     
     analogWrite(motorLCPin, min(255, max(0, motorL)));
     analogWrite(motorLCAPin, min(255, max(0, -motorL)));
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read(); 
    inputString += inChar;

    if (inChar == '\n')
    {
      stringComplete = true;
    } 
  }
}
