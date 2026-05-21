#include <SevSeg.h>
#include <dht_nonblocking.h>
#define tipoSensore  DHT_TYPE_11

SevSeg sevseg;

int pinBottone=24;
int statoBottone=0;
int  cambioDisplay=0;
int ultStatoBottone=0;


static const int pinSensore  = 22;
DHT_nonblocking sensoreDHT( pinSensore, tipoSensore );

void setup(  )
{
  pinMode(pinBottone, INPUT_PULLUP);
  Serial.begin(9600);
  byte  numCifre=4; //how many digits has the display
  byte pinCifre[]={10,11,12,13};  //digit pins
  byte pinSegmenti[]={9,2,3,5,6,8,7,4}; //segment pins from A to  G plus decimal point pin (last one)
  bool resistoriSuSegmenti=true;
  byte  CAoCC=COMMON_ANODE; //common cathode or common anode display
  sevseg.begin(CAoCC,  numCifre, pinCifre, pinSegmenti, resistoriSuSegmenti);
  sevseg.setBrightness(-10);
}

static  bool misuraAmbiente( float *temperatura, float *umidita )
{
  static unsigned  long tempo = millis( );

  //measures every 4 seconds
  if( millis( ) -  tempo > 4000)
  {
    if( sensoreDHT.measure( temperatura, umidita ) == true  )
    {
      tempo = millis( );
      return( true );
    }
  }

  return( false );
}


void loop( )
{
  float temperatura;
  float umidita;
  statoBottone=digitalRead(pinBottone);
  if (statoBottone!=ultStatoBottone)  //check if button is pressed
  {
    
    //change state if button is pressed
    if (statoBottone==1)
    { 
      if(cambioDisplay==1)
      {
        cambioDisplay=0;
      }
      else
      {
        cambioDisplay=1;  
      }      
    }
     
    //remember the new state
    ultStatoBottone=statoBottone;
  }
if(misuraAmbiente(&temperatura, &umidita)==true)
{};
  switch(cambioDisplay)  //switch display mode (temperature or humidity)
  {
    case 0:
    {
      sevseg.setNumberF(temperatura, 1);
      break;
    };
    case 1:
    {
      sevseg.setNumber(umidita);
      break;
    };
  }
  
  sevseg.refreshDisplay();
}
