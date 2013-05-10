#define cbi( sfr, bit ) ( _SFR_BYTE( sfr ) &= ~_BV( bit ) )
#define sbi( sfr, bit ) ( _SFR_BYTE( sfr ) |= _BV( bit ) )
 
volatile byte Timewaster;
 
void setup()
{
 
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
 
void loop()
{
 
    /* Wait for the conversion to complete. */
    while( !( ADCSRA & _BV( ADIF ) ) ){}
 
    /* Grab the result. */
    /* We need to grab ADCL first because accessing ADCH will discard ADCL. */
    byte Low = ADCL;
    unsigned short Left = ( ADCH << 8 ) | Low;  
   
    /* Clear the finished bit. */
    cbi( ADCSRA, ADIF );    
   
    if( UpdateAgo ) UpdateAgo--; else
    {
     
        sbi( ADMUX, MUX1 );  
   
        if( UpdateWhich ) sbi( ADMUX, MUX0 );    
     
        /* Give it four cycles to sink in. */
        Timewaster++;
        Timewaster--;  
        Timewaster++;
        Timewaster--;        
     
        /* Tell the ADC to get to work! */
        sbi( ADCSRA, ADSC );      
     
        /* Wait for the conversion to complete. */
        while( !( ADCSRA & _BV( ADIF ) ) ){}
   
        /* Grab the result. */
        /* We need to grab ADCL first because accessing ADCH will discard ADCL. */
        byte Low = ADCL;
       
        if( UpdateWhich )
        {
       
            LFODepth = ( ( ADCH << 8 ) | Low ) >> 2;  
           
            cbi( ADMUX, MUX0 );
           
            UpdateWhich = 0;
           
        }
        else
        {
       
            LFOSpeed = ( ( ADCH << 8 ) | Low ) >> 2;  
           
            UpdateWhich = 1;            
           
        }
       
        /* Clear the finished bit. */
        cbi( ADCSRA, ADIF );    
     
        cbi( ADMUX, MUX1 );
     
        UpdateAgo = 4096;
     
    }
   
    /* Give it four cycles to sink in. */
    Timewaster++;
    Timewaster--;  
    Timewaster++;
    Timewaster--;  
   
    /* Tell the ADC to get to work! */
    sbi( ADCSRA, ADSC );
   
    Buffer[ Ringstart ] = Left;
   
    Ringstart++;
   
    if( LFOSlow ) LFOSlow--; else
    {
   
      if( LFODirection )
      {
       
          LFO++;
         
          if( LFO == 255 ) LFODirection = 0;
       
      }
      else
      {
       
          LFO--;
         
          if( LFO <= LFODepth ) LFODirection = 1;
       
      }    
     
      LFOSlow = LFOSpeed;
     
    }
   
    byte Progress = Ringstart;
    Progress += LFO;
   
    Left += Buffer[ Progress ];
   
    /* Scale it up to full 16-bit. */
    Left <<= 5;
 
    analogWrite( 3, Left >> 8 );
    analogWrite( 11, Left & 255 );    
   
}
