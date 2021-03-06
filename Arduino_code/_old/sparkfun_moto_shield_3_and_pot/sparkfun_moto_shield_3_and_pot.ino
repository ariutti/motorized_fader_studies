/*
 * The shield that we are usign is the sparkfun monster moto shield
 * See the following link: https://www.sparkfun.com/products/10182.
 * The sketch is inspired from the official example code taken 
 * from Github page: https://github.com/sparkfun/Monster_Moto_Shield/tree/Hv12Fv10
 */

#include <Limulo_Motor.h>
Motor motor;

#define POT A4
#define THLOW 100
#define THHIGH 900
uint16_t pot;
bool DIRECTION = false;
int CURRENTTHRESHOLD = 16;
bool POLARITY = false;

float A, B;

/*
 * TODO:
 * 1. add protection on over current;
 * 2. add brake-on-touch;
 */


// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  motor.init();
  motor.enable(1);
  motor.disable(2);

  motor.setSpeed(95);
  //Serial.println( motor.getSpeed() );
  //motor.forward();  
  motor.stop();
  
  Serial.begin(9600);  // Initiates the serial to do the monitoring 
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

  A = 0.9:
  B = 1.0 - A;
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  pot = analogRead(POT);
  if( DIRECTION && pot < THLOW)
  {
    motor.stop();
    if(POLARITY)
      motor.forward();
    else
      motor.reverse();
    DIRECTION = !DIRECTION;
  }
  else if(!DIRECTION && pot > THHIGH )
  {
    motor.stop();
    if(POLARITY)
      motor.reverse();
    else
      motor.forward();
    DIRECTION = !DIRECTION;
  }
  //Serial.println(pot);
  
  float v = motor.getCurrent(1);
  
  Serial.println(v);
  if(v >= CURRENTTHRESHOLD)
  {
    //Serial.println("Current over threshold");
    motor.stop();
  }
  
  
}



// SERIAL PRINT //////////////////////////////////////////////////////////////
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
    //Serial.println("Invalid option entered.");
  }
}

