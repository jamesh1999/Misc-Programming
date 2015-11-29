#include <avr/io.h>
#include <avr/interrupt.h> 

volatile uint8_t timer;
volatile uint8_t second;
volatile uint8_t bits;

uint8_t year;
uint8_t month;
uint8_t day;
uint8_t weekday;
uint8_t hour;
uint8_t minute;
volatile uint8_t delta_minutes;

void setup() {
  Serial.begin(4800);
  DDRB |= _BV(PB5) | _BV(PB1);

  TCCR0A = _BV(WGM01);
  OCR0A  =  0xF9;    // 16mhz / ((249+1) * 256 prescale) = 250hz
  TCCR0B = _BV(CS02);  // CTC OCR0A TOP & 256 prescaler
  TIMSK0 = _BV(OCIE0A);// enable compare match interrupt
  
  ICR1 = 266; // Set OCR1C to top p. 91 (60kHz)
  TCCR1A = _BV(WGM11) | _BV(COM1A0) | _BV(COM1B0);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);    /* Set clock prescaler to 8   */ 
  
  sei();  // Enable interupts
}

ISR(TIMER0_COMPA_vect){  
  ++timer;      // Advance timer

  if (timer == 250) {  // Check to see if at end of second
    timer = 0;    // If so reset timer
    
    ++second;     // Advance data slot in minute data packet
    if (second == 60) {
      second = 0; // Reset slot to 0 if at 60 seconds
      ++delta_minutes; // Advance minute count
    }

    switch (second) {
    
    case 0 : { bits = 31;break;} 

    //Year
    case 17: { bits = 0;break;} 
    case 18: { bits = 0;break;} 
    case 19: { bits = 0;break;}
    case 20: { bits = 2;break;}
    case 21: { bits = 0;break;}
    case 22: { bits = 2;break;}
    case 23: { bits = 0;break;}
    case 24: { bits = 2;break;}

    //Month
    case 25: { bits = 2;break;}
    case 26: { bits = 0;break;}  
    case 27: { bits = 0;break;}  
    case 28: { bits = 0;break;}
    case 29: { bits = 2;break;}  

    //Day
    case 30: { bits = 0;break;}  
    case 31: { bits = 0;break;}  
    case 32: { bits = 0;break;}  
    case 33: { bits = 0;break;}
    case 34: { bits = 2;break;}
    case 35: { bits = 2;break;}

    //Weekday
    case 36: { bits = 0;break;}
    case 37: { bits = 2;break;}
    case 38: { bits = 0;break;}

    //Hour
    case 39: { bits = 0;break;}
    case 40: { bits = 0;break;}
    case 41: { bits = 2;break;}
    case 42: { bits = 0;break;}
    case 43: { bits = 0;break;}
    case 44: { bits = 2;break;}

    //Minute
    case 45: { bits = 0;break;}
    case 46: { bits = 0;break;}
    case 47: { bits = 0;break;}
    case 48: { bits = 0;break;}
    case 49: { bits = 0;break;}
    case 50: { bits = 0;break;}
    case 51: { bits = 0;break;}
    
    case 52: { bits = 1;break;}
    case 53: { bits = 3;break;}
    case 54: { bits = 3;break;}
    case 55: { bits = 7;break;}
    case 56: { bits = 3;break;}
    case 57: { bits = 7;break;}
    case 58: { bits = 3;break;}
    case 59: { bits = 1;break;}
    
    default: { break;} 
  }
  bits |= 1;
  }

  //Get bit of signal to use & set output
  if(timer % 25 == 0)
  {
  if((bits >> (timer / 25)) & 1)
    TCCR1A &= ~(_BV(COM1A0) | _BV(COM1B0));
  else
    TCCR1A |= _BV(COM1A0) | _BV(COM1B0);
  }
}

void loop(){
  Serial.print(ICR1-);
  while(1){}
}
