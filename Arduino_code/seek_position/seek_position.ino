/*
 * The shield that we are usign is the sparkfun monster moto shield
 * See the following link: https://www.sparkfun.com/products/10182.
 * The sketch is inspired from the official example code taken 
 * from Github page: https://github.com/sparkfun/Monster_Moto_Shield/tree/Hv12Fv10
 */

#include <Limulo_Motor.h>
Motor motor;

#define POT A4
uint16_t pot, destination;
int MOTORSPEED = 85;
int LOWSTOP = 10;
int HIGHSTOP = 1013;

enum {
  STOP = 0,
  SEEKFORWARD,
  SEEKREVERSE
} state;


// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  motor.init();
  motor.enable(1);
  motor.disable(2);
  motor.setSpeed(MOTORSPEED);
  motor.stop();
  
  Serial.begin(9600);  // Initiates the serial to do the monitoring 
  state = STOP;
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  pot = analogRead(POT);
  if(state == SEEKFORWARD )
  {
    Serial.println(pot);
    if(pot >= destination)
    {
      // we are almost arrived to the destination
      // stop the motor and change state
      Serial.println("Destination reached!");
      motor.stop();
      state = STOP;
    }
  } 
  else if(state == SEEKREVERSE )
  {
    Serial.println(pot);
    if(pot <=destination)
    {
      // we are almost arrived to the destination
      // stop the motor and change state
      Serial.println("Destination reached!");
      motor.stop();
      state = STOP;
    }   
  }
  else if(state == STOP )
  {
    // do nothing for now
  }
  delay(10);
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
  else if( user_input == 'r' && state == STOP)
  {
    destination = random(LOWSTOP, HIGHSTOP);
    pot = analogRead(POT);
    int distance = destination - pot;
    Serial.print("Destination: ");Serial.println(destination);
    Serial.println(distance);
    
    if(distance > 0)
    {
      Serial.println("going Forward");
      motor.forward();
      state = SEEKFORWARD;
    }
    else if(distance < 0)
    {
      Serial.println("going reverse");
      motor.reverse();
      state = SEEKREVERSE;
    }
  }
  else
  {
    //Serial.println("Invalid option entered.");
  }
}

