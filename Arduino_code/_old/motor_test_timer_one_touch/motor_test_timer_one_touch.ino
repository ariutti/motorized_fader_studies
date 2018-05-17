/*
 * This sketch is using:
 * - Limulo_Motor library: to control the motor via the SparkfunMotoShield;
 * - LimuloMPR121 library: to check touches from the user un the fader cap; 
 * - TimerOne library: to make the motor go slower.
 */

// motor stuff
#include <TimerOne.h>
#include "Limulo_Motor.h"

Motor motor;
unsigned int timeGap = 10000;// in uSecs
unsigned int motorSpeed = 30;

// touch stuff
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
  Serial.begin(9600);  // Initiates the serial to do the monitoring 
  while(!Serial) { delay(10); }
  
  // motor stuff
  motor.init(1000);
  motor.enable(1);
  motor.disable(2);
  motor.setSpeed( motorSpeed );
  //Serial.println( motor.getSpeed() );
  //motor.forward();  
  motor.stop();
  /*
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. BRAKE");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();
  */

  //Serial.println("Time One initialization");
  Timer1.initialize( timeGap );
  Timer1.attachInterrupt(swapMotorOnOff);

  // touch stuff
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  mpr.cap = Limulo_MPR121();
  // mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D 
  mpr.addr = FIRST_MPR_ADDR;
  
  // Look for the MPR121 chip on the I2C bus
  if ( !mpr.cap.begin( mpr.addr ) ) {
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
  //mpr.cap.setThresholds( 10, 6 );
  //mpr.setDebounces(1, 1);
  
  bToPlotter=true;
}

volatile boolean motorStatus = true;
volatile unsigned int motorChangeCount = 0;

/*
 * This function is building a sort of PWM
 * to further control the speed of the motor.
 * 
 * The function is usign the TimerOne library 
 * with the interrupt feature.
 * 
 * ||___||___||___||___||___ (25% duty cycle)
 * 
 * I've empirically found that the values which seems 
 * to work better are:
 * 10000 uS for the timeGap
 * 30 for the motorSpeed;
*/
void swapMotorOnOff(void)
{
  if(motorChangeCount == 0)
  { 
    motorStatus = true;   
    motor.enable(1);
  }
  else if(motorChangeCount == 1)
  {
    motorStatus = false;
    motor.disable(1);
  }
  motorChangeCount ++;
  motorChangeCount = motorChangeCount%4;
}


/************************************************************************************
 * LOOP
 ***********************************************************************************/
 void loop() 
{
  // serial stuff
  if(Serial.available() > 0) {
    // Read user input and trigger appropriate function
    char user_input = Serial.read(); 

    // motor serial data *****************************
    if (user_input =='1') {
       motor.stop();
    } 
    else if(user_input =='2') {
      motor.forward();
    } 
    else if(user_input =='3') {
      motor.reverse();
    } 
    else if(user_input =='+') {
      motor.increaseSpeed();
      //Serial.println( motor.getSpeed() );
    } 
    else if(user_input =='-') {
      motor.decreaseSpeed();
      //Serial.println( motor.getSpeed() );
    } 
    // touch serial data *****************************
    else if (user_input == 'o') {
      bToPlotter = true;
    } 
    else if (user_input == 'c') {
      bToPlotter = false;
    } 
    else if (user_input == 'r') {
      // reset the MPR
      mpr.cap.reset();
    }
  }

  // motor stuff
  boolean motorStatusCopy;
  unsigned int motorChangeCountCopy;
  noInterrupts();
  motorStatusCopy = motorStatus;
  motorChangeCountCopy = motorChangeCount;
  interrupts();

  //Serial.print("motorChangeCount = ");  Serial.println(motorChangeCount);

  // touch stuff
 
  // examine the state of the MPR
  mpr.currtouched = mpr.cap.touched(); // Get the currently touched pads
  // cycle through all the electrodes
  for(int j=0; j<NPADS; j++)
  {
    if (( mpr.currtouched & _BV(j)) && !(mpr.lasttouched & _BV(j)) ) 
    {
      // pad 'j' has been touched
      //Serial.print("Pad "); Serial.print(j); Serial.println("\ttouched!");
      mpr.bPadState[j] = true;
      digitalWrite(LED, mpr.bPadState[j]);
      motor.forward();
    }
    if (!(mpr.currtouched & _BV(j)) && (mpr.lasttouched & _BV(j)) ) 
    {
      // pad 'i' has been released
      //Serial.print("Pad "); Serial.print(j); Serial.println("\treleased!");
      mpr.bPadState[j] = false;
      digitalWrite(LED, mpr.bPadState[j]);
      motor.stop();
    }
  }
  // reset our state
  mpr.lasttouched = mpr.currtouched;

  // Send Serial data
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
      Serial.print(base); Serial.print("\t");
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
}


