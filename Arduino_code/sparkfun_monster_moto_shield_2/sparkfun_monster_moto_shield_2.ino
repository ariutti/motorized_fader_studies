/*
 * The shield that we are usign is the sparkfun monster moto shield
 * See the following link: https://www.sparkfun.com/products/10182.
 * The sketch is inspired from the official example code taken 
 * from Github page: https://github.com/sparkfun/Monster_Moto_Shield/tree/Hv12Fv10
 */

#include <Limulo_Motor.h>

Motor motor;

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  motor.init();
  motor.enable(1);
  motor.disable(2);
  
  Serial.begin(9600);  // Initiates the serial to do the monitoring 
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. BRAKE");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  motor.printCurrents();
}


void serialEvent()
{
  char user_input = Serial.read(); // Read user input and trigger appropriate function
    
  if (user_input =='1')
  {
     motor.stop();
  }
  else if(user_input =='2')
  {
    motor.forward();
  }
  else if(user_input =='3')
  {
    motor.reverse();
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
    Serial.println("Invalid option entered.");
  }
}

