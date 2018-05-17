/*
 * The shield that we are usign is the sparkfun monster moto shield
 * See the following link: https://www.sparkfun.com/products/10182.
 * The sketch is inspired from the official example code taken 
 * from Github page: https://github.com/sparkfun/Monster_Moto_Shield/tree/Hv12Fv10
 * 
 * This sketch also make use of the "TimerOne" library.
 * See this link for more information: https://www.pjrc.com/teensy/td_libs_TimerOne.html
 */

#include "Limulo_Motor.h"
#include <TimerOne.h>
Motor motor;
unsigned int timeGap = 10000;// in uSecs
unsigned int motorSpeed = 30;

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  motor.init(1000);
  motor.enable(1);
  motor.disable(2);

  motor.setSpeed( motorSpeed );
  //Serial.println( motor.getSpeed() );
  //motor.forward();  
  motor.stop();
  
  Serial.begin(9600);  // Initiates the serial to do the monitoring 
  while(!Serial) { delay(10); }
  
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. BRAKE");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();

  Serial.println("Time One initialization");
  Timer1.initialize( timeGap );
  Timer1.attachInterrupt(swapMotorOnOff); // swapMotorOnOff to run every 0.15 seconds
  
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



// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  if(Serial.available() > 0)
  {
    char user_input = Serial.read(); // Read user input and trigger appropriate function
      
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
      Serial.println( motor.getSpeed() );
    }
    else if(user_input =='-') {
      motor.decreaseSpeed();
      Serial.println( motor.getSpeed() );
    }
    else {
      Serial.println("Invalid option entered.");
    }
  }

  boolean motorStatusCopy;
  unsigned int motorChangeCountCopy;
  noInterrupts();
  motorStatusCopy = motorStatus;
  motorChangeCountCopy = motorChangeCount;
  interrupts();

  //Serial.print("motorChangeCount = ");  Serial.println(motorChangeCount);

  delay(100);
  

  
  
  /*
  motor.forward();
  Serial.println("forward");
  delay(2000);
  float v = motor.getCurrent(1);
  Serial.println(v);
  motor.reverse();
  Serial.println("reverse");
  delay(2000);
  v = motor.getCurrent(1);
  Serial.println(v);
  
  if(v >= CURRENTTHRESHOLD)
  {
    //Serial.println("Current over threshold");
    motor.stop();
  }
  */
}


