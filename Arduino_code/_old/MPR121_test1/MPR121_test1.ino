#include <Wire.h>
#include "Limulo_MPR121.h"

#define LED 13

#define NPADS 1
#define FIRST_MPR_ADDR 0x5A

struct mpr121 
{
  Limulo_MPR121 cap;
  uint8_t addr;
  // Save an history of the pads that have been touched/released
  uint16_t lasttouched = 0;
  uint16_t currtouched = 0;
  boolean bPadState[12];
} mpr;


boolean bToPlotter;
int delayTime = 50;

/************************************************************************************
 * SETUP
 ***********************************************************************************/
void setup()
{
  // open the I2C communication
  Serial.begin(9600, SERIAL_8N1);
  /*
  while(!Serial) {
    delay(10);
  }
  Serial.println("continue!");
  */

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  mpr.cap = Limulo_MPR121();
  // mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D 
  mpr.addr = FIRST_MPR_ADDR;
  
  // Look for the MPR121 chip on the I2C bus
  if ( !mpr.cap.begin( mpr.addr ) )
  {
    //Serial.println("MPR121 not found, check wiring?");
    while (1);
  }

  // initialize the array of booleans
  for(int j=0; j<12; j++)
  {
    mpr.bPadState[j] = false;
  }
  
  // possibly initialize the mpr with some initial settings
  //mpr.setRising( 1, 8, 1, 0);
  //mpr.setFalling( 1, 1, 2, 1);
  mpr.cap.setTouched( 1, 255, 255);
  mpr.cap.setThresholds( 10, 6 );
  //mpr.setDebounces(1, 1);
  

  bToPlotter=true;
}

/************************************************************************************
 * LOOP
 ***********************************************************************************/
void loop()
{
  // TODO: Add here serial communication section in order 
  // to discard the serial event callback function

  
  if(Serial.available()>0)
  {
    byte b = Serial.read();
    if (b == 'o')
      bToPlotter = true;
    else if (b == 'c')
      bToPlotter = false;
    else if (b == 'r')
    {
      // reset the MPR
      mpr.cap.reset();
    }
  }
  
  // examine the state of the MPR
  mpr.currtouched = mpr.cap.touched(); // Get the currently touched pads
  // cycle through all the electrodes
  for(int j=0; j<NPADS; j++)
  {
    if (( mpr.currtouched & _BV(j)) && !(mpr.lasttouched & _BV(j)) ) 
    {
      // pad 'j' has been touched
      Serial.print("Pad "); Serial.print(j); Serial.println("\ttouched!");
      mpr.bPadState[j] = true;
      digitalWrite(LED, mpr.bPadState[j]);
    }
    if (!(mpr.currtouched & _BV(j)) && (mpr.lasttouched & _BV(j)) ) 
    {
      // pad 'i' has been released
      Serial.print("Pad "); Serial.print(j); Serial.println("\treleased!");
      mpr.bPadState[j] = false;
      digitalWrite(LED, mpr.bPadState[j]);
    }
  }
  // reset our state
  mpr.lasttouched = mpr.currtouched;

  // Send Serial data ////////////////////////////////////////////////////////////
  if(bToPlotter)
  {
    // Send data via serial:
    // 1. First send an address. Address byte have a 128 <= value < 255.
    //    Each address identifies a specific upcoming value;
    // 2. then send that value. Consider the value is a 10 bit but we must
    //    send it inside a 7bit packet. So made 3 right bit shift (/8). 
    //    This way we will loose some precision but this is not important.

    // cycle all the electrodes
    for(int j=0; j<NPADS; j++) 
    {
      int filt = mpr.cap.filteredData(j);
      int base = mpr.cap.baselineData(j);
      //byte b = (1<<7) | (i<<5) | (j<<1) | mpr.bPadState[j];
      //Serial.write(b); // send address & touched
      //Serial.write(base / 8); // send base value
      //Serial.write(filt / 8); // send filtered value

      // below there are three lines for the Arduino Serial plotter
      Serial.print(filt); Serial.print("\t");
      Serial.print(base);Serial.print("\t");
      Serial.println();

      // below some line for the Processing plotter
      //Serial.print( mpr.bPadState[j] ); Serial.print(",");
      //Serial.print(filt); Serial.print(",");
      //Serial.print(base);
      //Serial.println();


      
      /*
      Serial.print(base); Serial.print("\t");
      Serial.print(mpr[i].bPadState[j]); Serial.println("\t");
      */
    }
  }
  

  delay(delayTime); // put a delay so it isn't overwhelming
}

/************************************************************************************
 * SERIAL EVENT
 ***********************************************************************************
void serialEvent()
{
  byte b = Serial.read();
  if (b == 'o')
    bToPlotter = true;
  else if (b == 'c')
    bToPlotter = false;
  else if (b == 'r')
  {
    // reset all the MPR
    for(int i=0; i<NMPR; i++)
      mpr[i].cap.reset();
  }
}
*/
