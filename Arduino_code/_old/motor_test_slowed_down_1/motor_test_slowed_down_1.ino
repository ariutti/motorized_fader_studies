/*
 * A sketch to test if it is possible to move the motor 
 * slower.
 */

#include <Limulo_Motor.h>
Motor motor;

#define POT A4
#define TH_LOW 100
#define TH_HIGH 900
uint16_t pot;
int CURRENTTHRESHOLD = 16;
int MOTORSPEED = 100; //

enum {
  FORWARD,
  REVERSE,
  QUIET
} state;

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  motor.init(10000); // (10ms is the best tradeoff)
  motor.enable(1);
  motor.disable(2);

  motor.setSpeed(MOTORSPEED);
  motor.stop();

  pot = analogRead(POT);
  if(pot>512)
  {
    state = FORWARD;
    motor.forward();
  }
  else
  {
    state = REVERSE;
    motor.reverse();
  }  
  Serial.begin(115200);  // Initiates the serial to do the monitoring 
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  if(state==FORWARD || state==REVERSE)
    motor.updatePWM(1);

  pot = analogRead(POT);
  if(state==FORWARD && pot > TH_HIGH)
  {
    //Serial.println("moving reverse");
    state = REVERSE;
    motor.stop();
    motor.reverse();
  } 
  else if(state==REVERSE && pot < TH_LOW)
  {
    //Serial.println("moving forward");
    state = FORWARD;
    motor.stop();
    motor.forward();
  }

  // check current
  float v = motor.getCurrent(1);  
  Serial.println(v);
  if(v >= CURRENTTHRESHOLD)
  {
    //Serial.println("Current over threshold");
    motor.stop();
    state = QUIET;
  }
}



// SERIAL PRINT //////////////////////////////////////////////////////////////
void serialEvent()
{
  char user_input = Serial.read(); // Read user input and trigger appropriate function
    
  if (user_input =='1')
  {
     motor.stop();
     state = QUIET;
  }
  else if(user_input =='2')
  {
    motor.forward();
    state = FORWARD;
  }
  else if(user_input =='3')
  {
    motor.reverse();
    state = REVERSE;
  }
  else if(user_input =='+')
  {
    motor.increaseSpeed();
  }
  else if(user_input =='-')
  {
    motor.decreaseSpeed();
  }
  else
  {
    //Serial.println("Invalid option entered.");
  }
}

