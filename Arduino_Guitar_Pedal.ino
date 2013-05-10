#include "dsp.h"

#define cbi( sfr, bit ) ( _SFR_BYTE( sfr ) &= ~_BV( bit ) )
#define sbi( sfr, bit ) ( _SFR_BYTE( sfr ) |= _BV( bit ) )

volatile byte Timewaster;

// create an array for the delay
byte array[2000]; 

//define variables
int j;
int fx;
int mode;
int value50;
int value300;
int value10000;
int delayed;

void setup() {
  setupIO();
  
  //set initial values
  j = 50;
  value50 = 50;
  value300 = 300;
  value10000 = 10000;
  delayed = 0;
  
 /* Speed up timer 2 to make pins 3 and 11 output a blistering fast PWM. */
  TCCR2B = TCCR2B & 0b11111000 | 1;
  TCCR2A &= ~B11;
  TCCR2A |=  B011;
  pinMode( 3, OUTPUT );  
  pinMode( 11, OUTPUT );  
 
  /* Do the same for timer 0.  WARNING: Much of the Arduino library relies on timer 0 and you will find some functions output incorrect values.  Look up the full effects and make sure you don't need it. */
  TCCR0B = TCCR0B & 0b11111000 | 1;
  TCCR0A &= ~B11;
  TCCR0A |=  B011;
  pinMode( 5, OUTPUT );  
  pinMode( 6, OUTPUT );      
 
  analogWrite( 3, 0 );
  analogWrite( 11, 0 );    
  analogWrite( 5, 0 );
  analogWrite( 6, 0 );    
 
  /* Set the reference level to 1.1v */
  sbi( ADMUX, REFS0 );
  sbi( ADMUX, REFS1 );  
 
  /* Increase our input speed - prescaler to /8 */  
  cbi( ADCSRA, ADPS2 );
  sbi( ADCSRA, ADPS1 );
  sbi( ADCSRA, ADPS0 );  
 
  /* Select A0. */
  cbi( ADMUX, MUX0 );
  cbi( ADMUX, MUX1 );
  cbi( ADMUX, MUX2 );
  cbi( ADMUX, MUX3 );  
 
  /* Turn on the ADC. */
  sbi( ADCSRA, ADEN );
 
  /* Give it four cycles to sink in. */
  Timewaster++;
  Timewaster--;  
  Timewaster++;
  Timewaster--;  
 
  /* Tell the ADC to get to work! */
  sbi( ADCSRA, ADSC );
 
  /* Our LED. */
  pinMode( 13, OUTPUT ); 
 
}
unsigned short Buffer[ 256 ];
byte Ringstart = 0;
 
byte LFO = 0;
boolean LFODirection = 0;
 
 
unsigned short UpdateAgo = 0;
boolean UpdateWhich = 0;
byte LFOSpeed = 0;
byte LFOSlow = 0;
byte LFODepth = 0;


void loop() {
  
    //check status of the effect potentiometer and rotary switch
    readKnobs();
  
    // *************
    // ***bitcrush**
    // *************
    if(mode == 6){
      value300 = 1 + ((float) fx / (float) 3);        
      if(delayed > value300) {  
        byte input = analogRead(left);
        input = (input >> 6 << 6);
        //output(left, input);
        analogWrite(3, input>>2);
        analogWrite(11,(input&B11)<<6);
        delayed = 0;
       } 
       delayed++;

    }
    
    
    // ***************
    // ***Overdrive***
    // ***************
    if(mode == 7){
      value50 = 1 + ((float) fx / (float) 20);    
      byte input = analogRead(left);
      input = (input * value50); 
      output(left, input);

    }


    //  *************************
    //  ***short crunchy delay***
    //  *************************

    if(mode == 9){
      for(int i = 0; i < 2000; i ++) { // set up a loop    
        //array[i] = array[i] + array[i - 1]; //removes noise and some delay
        output(left, array[i]);
        array[i] = analogRead(left);
      }
    }


    //  **********************
    //  ***clean helicopter***
    //  **********************
    if(mode == 10){
      value10000 = fx * 10;  
      if(delayed > value10000) { 
        for(int i = 0; i < 2000; i ++) { // set up a loop    
          array[i] = array[i] + array[i - 1]; //removes noise and delay
          output(left, array[i]);
          array[i] = analogRead(left);
        }
        delayed = 0;
      }
      delayed++;
    }
    
    //  ********************
    //  ***ring modulator***
    //  ********************
    if(mode == 11){
     value50 = 1 + ((float) fx / (float) 20);    
     byte input = analogRead(left);
     input = (input * j);
     output(left, input);
     
     j = j - 1;
     if (j <= 0) {
       j = value50;
     }

    }
    
    //  ************************
    //  ***YOUR AWESOME SOUND***
    //  ************************
    if(mode == 12){
          
      //************************************
      //insert your awesome effect code here
      //************************************
  
    }
    
}

void readKnobs(){
  //read the rotary switch
  //and determine which effect is selected
  //dividing by 75 ensures proper discrete values
  //for if statements above
  mode = analogRead(1);
  mode = mode / 75;
    
  //reads the effects pot to adjust
  //the intensity of the effects above  
  fx = analogRead(3);

}
