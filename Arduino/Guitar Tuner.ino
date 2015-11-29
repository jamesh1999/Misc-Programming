#include <SoftwareSerial.h>
#include <avr/interrupt.h>

#define ACCURACY 3
#define STRING1 330
#define STRING2 247
#define STRING3 196
#define STRING4 147
#define STRING5 110
#define STRING6 82
#define MAX_INDEX 15

const int buffer = ACCURACY * 15;

const int clockPin = 2;
const int dataPin = 3;
const byte ampInputPin = 5;


//data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;//keeps time and sends vales to store in timer[] occasionally
int timer[MAX_INDEX];//storage for timing of events
int slope[MAX_INDEX];//storage for slope of events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage for period of wave
byte index = 0;//current storage index
float frequency = 0;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data

//variables for decided whether you have a match
byte noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 3;//slope tolerance- adjust this if you need
const int timerTol = 8;

SoftwareSerial serial(13, 4);



void setup() {
  cli();//diable interrupts
  ADMUX = B01100000 | ampInputPin;
  ADCSRA = B11101100; //enable ADC, enable interrupts, start measurements, auto trigger, 1MHz prescaler
  ADCSRB = B00000000;
  sei();//enable interrupts
  
  serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  setShiftRegister(B00000001);
  delay(100);
  setShiftRegister(B00000011);
  delay(100);
  setShiftRegister(B00000111);
  delay(100);
  setShiftRegister(B00001111);
  delay(100);
  setShiftRegister(B00011111);
  delay(100);
  setShiftRegister(B00111111);
  delay(100);
  setShiftRegister(B01111111);
  delay(100);
  setShiftRegister(B11111111);
  delay(100);
  setShiftRegister(B01111111);
  delay(100);
  setShiftRegister(B00111111);
  delay(100);
  setShiftRegister(B00011111);
  delay(100);
  setShiftRegister(B00001111);
  delay(100);
  setShiftRegister(B00000111);
  delay(100);
  setShiftRegister(B00000011);
  delay(100);
  setShiftRegister(B00000001);
  delay(100);
  setShiftRegister(B00000000);
}

ISR(ADC_vect) {
  prevData = newData;
  newData = ADCH;
  
  if (prevData < 127 && newData >=127){//if increasing and crossing midpoint
    newSlope = newData - prevData;//calculate slope
    if(abs(newSlope - maxSlope) < slopeTol){//if slopes are ==
      //record new data and reset time
      slope[index] = newSlope;
      timer[index] = time;
      time = 0;
      if (index == 0){//new max slope just reset
        noMatch = 0;
        ++index;//increment index
      }
      
      else if (abs(timer[0] - timer[index])<timerTol && abs(slope[0] - newSlope)<slopeTol){//if timer duration and slopes match
        //sum timer values
        totalTimer = 0;
        for (byte i=0;i<index;i++){
          totalTimer += timer[i];
        }
        period = totalTimer;//set period
        //reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1;//set index to 1
        noMatch = 0;
      }
      else{//crossing midpoint but not match
        ++index;//increment index
        if (index >= MAX_INDEX){
          reset();
        }
      }
    }
    else if (newSlope > maxSlope){//if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0;//reset clock
      noMatch = 0;
      index = 0;//reset index
    }
    else{//slope not steep enough
      ++noMatch;//increment no match counter
      if (noMatch >= MAX_INDEX){
        reset();
      }
    }
  }
  
  ++time;//increment timer at rate of 70kHz
}

void reset(){//clean out some variables
  index = 0;//reset index
  noMatch = 0;//reset match couner
  maxSlope = 0;//reset slope
}

void setShiftRegister(byte data)
{
  for(int i = 0; i < 8; ++i)
  {
     if(data & B10000000)
     {
       digitalWrite(dataPin, HIGH);
     }
     else
     {
       digitalWrite(dataPin, LOW);
     }
     digitalWrite(clockPin, HIGH);
     digitalWrite(clockPin, LOW);
     
     data <<= 1;
  } 
}

void displayNumber(unsigned num, boolean writeExponent)
{
  String st = String(num, DEC);
  int extra = st.length() - 2;
  for(;extra < 0; ++extra)
  {
    st = '0' + st; 
  }
  
  //Handle exponent
  if(extra <= 8 && writeExponent)
  {
    byte data = 0;
    for(int i = 0; i < extra; ++i)
    {
      data <<= 1; 
      data += 1;
    }
    setShiftRegister(data); 
  }

  //Handle most significant figures
  unsigned chr0 = st[1] - '0';
  unsigned chr1 = st[0] - '0';

  if(chr0 & 2)
  {
    digitalWrite(9, HIGH);
  }
  else
  {
    digitalWrite(9, LOW); 
  }
  if(chr0 & 4)
  {
    digitalWrite(10, HIGH);
  }
  else
  {
    digitalWrite(10, LOW); 
  }
  if(chr0 & 8)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW); 
  }
  if(chr0 & 1)
  {
    digitalWrite(12, HIGH);
  }
  else
  {
    digitalWrite(12, LOW); 
  }

  if(chr1 & 2)
  {
    digitalWrite(8, HIGH);
  }
  else
  {
    digitalWrite(8, LOW); 
  }
  if(chr1 & 4)
  {
    digitalWrite(7, HIGH);
  }
  else
  {
    digitalWrite(7, LOW); 
  }
  if(chr1 & 8)
  {
    digitalWrite(6, HIGH);
  }
  else
  {
    digitalWrite(6, LOW); 
  }
  if(chr1 & 1)
  {
    digitalWrite(5, HIGH);
  }
  else
  {
    digitalWrite(5, LOW); 
  }
}

void loop() {
  delay(200);
  if(digitalRead(0))
  {
    displayNumber(unsigned(newData), true); 
  }
  else
  {
    frequency = 76924/float(period);

    if(digitalRead (1))
    {
      displayNumber(unsigned(frequency), false);
      
      int aim = 0;
      if(-buffer <= frequency - STRING1 && frequency - STRING2 <= buffer)
      {
        aim = STRING1;
      }
      else if(-buffer <= frequency - STRING2 && frequency - STRING2 <= buffer)
      {
        aim = STRING2;
      }
      else if(-buffer <= frequency - STRING3 && frequency - STRING2 <= buffer)
      {
        aim = STRING3;
      }
      else if(-buffer <= frequency - STRING4 && frequency - STRING2 <= buffer)
      {
        aim = STRING4;
      }
      else if(-buffer <= frequency - STRING5 && frequency - STRING2 <= buffer)
      {
        aim = STRING5;
      }
      else if(-buffer <= frequency - STRING6 && frequency - STRING2 <= buffer)
      {
        aim = STRING6;
      }

      if(frequency < aim - ACCURACY * 7)
      {
        setShiftRegister(B00000001);
      }
      else if(frequency < aim - ACCURACY * 3)
      {
        setShiftRegister(B00000010);
      }
      else if(frequency < aim - ACCURACY)
      {
        setShiftRegister(B00000100);
      }
      else if(frequency < aim)
      {
        setShiftRegister(B00001000);
      }
      else if(frequency == aim)
      {
        setShiftRegister(B00011000);
      }
      else if(frequency > aim + ACCURACY * 7)
      {
        setShiftRegister(B10000000);
      }
      else if(frequency > aim + ACCURACY * 3)
      {
        setShiftRegister(B01000000);
      }
      else if(frequency > aim + ACCURACY)
      {
        setShiftRegister(B00100000);
      }
      else if(frequency > aim)
      {
        setShiftRegister(B00010000);
      }
    }
    else
    {
      displayNumber(unsigned(frequency), true); 
    }
  }
}

