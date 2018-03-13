// Master State Machine ******************/
enum {
  MASTER_TOUCH = 0,
  MASTER_WAIT4STANDBY,
  MASTER_SEEK
} master_state;

// Motor / Potentiometer stuff ***********/
#include <Limulo_Motor.h>
Motor motor;

#define POT A4
uint16_t pot, destination;
int LOWSTOP = 20;
int HIGHSTOP = 1000;
int MOTORSPEED = 85;

enum {
  SEEK_STOP = 0,
  SEEK_FORWARD,
  SEEK_REVERSE
} seek_state;

// Touch stuff ***************************/
#include <ADCTouch.h>
int ref; //reference values to remove offset
const int TOUCHTH = 60;
bool TOUCHED = false;
int value;

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  Serial.begin(9600);  // Initiates the serial to do the monitoring 

  motor.init();
  motor.enable(1);
  motor.disable(2);
  motor.setSpeed(MOTORSPEED);
  motor.stop();
  master_state = MASTER_SEEK;
  seek_state = SEEK_STOP;
  chooseNewDestination();
  resetTouch();
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  pot = analogRead(POT);
  Serial.println(pot);
  /*
  switch( master_state )
  {
    case 0:
      Serial.print("TOUCH: ");
      Serial.println(value);
      break;
    case 1:
      Serial.println("WAIT4STANBY");
      break;
    case 2:
      Serial.println("SEEK");
      break;
    default:
      // do nothing
      break;
  }
  */

  // Master state machine ****************/
  if(master_state == MASTER_TOUCH)
  {
    //Serial.println("Debug: master touch case");
    // check if there's a release
    if(!readTouch())
    {
      //motor.stop(); //redundant
      master_state = MASTER_WAIT4STANDBY;
      // exit the loop now!
      return;
    }
    
  }
  else if(master_state == MASTER_WAIT4STANDBY)
  {
    //Serial.println("Debug: master wait for standby case;");
    // check if there's a touch
    if(readTouch())
    {
      motor.stop();
      master_state = MASTER_TOUCH;
      // exit the loop now!
      return;
    }
  } 
  else if(master_state == MASTER_SEEK)
  { 
    //Serial.println("Debug: master seek case;");

    // check if there's a touch
    if(readTouch())
    {
      motor.stop();
      master_state = MASTER_TOUCH;
      // exit the loop now!
      return;
    }

    if( isArrived() )
    {
      chooseNewDestination();
    }
    
  } // end of MASTER_SEKK state
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
  else if( user_input == 'r' && seek_state == SEEK_STOP)
  {
    chooseNewDestination();
  }
  else if(user_input == 's' )
  {
    if(master_state == MASTER_WAIT4STANDBY)
    {
      chooseNewDestination();
      master_state = MASTER_SEEK;
    }
  }
  else
  {
    //Serial.println("Invalid option entered.");
  }
}

// Seeking ///////////////////////////////////////////////////////////////////
void chooseNewDestination()
{
  destination = random(LOWSTOP, HIGHSTOP);
  pot = analogRead(POT);
  int distance = destination - pot;
  //Serial.print("pot: "); Serial.print(pot);
  //Serial.print("\tdest: "); Serial.print(destination);
  //Serial.print("\tdist: "); Serial.println(distance);
  
  if(distance > 0)
  {
    //Serial.println("going Forward");
    motor.forward();
    seek_state = SEEK_FORWARD;
  }
  else if(distance < 0)
  {
    //Serial.println("going reverse");
    motor.reverse();
    seek_state = SEEK_REVERSE;
  }
  else
  {
    chooseNewDestination();
  }
}

bool isArrived()
{
  if(seek_state == SEEK_FORWARD ) // sub-state machine (seeking)
  {
    //Serial.println(pot);
    if(pot >= destination)
    {
      // we are almost arrived to the destination
      // stop the motor and change state
      //Serial.println("Destination reached!");
      motor.stop();
      seek_state = SEEK_STOP;
      return true;
    }
  } 
  else if(seek_state == SEEK_REVERSE )
  {
    //Serial.println(pot);
    if(pot <=destination)
    {
      // we are almost arrived to the destination
      // stop the motor and change state
      //Serial.println("Destination reached!");
      motor.stop();
      seek_state = SEEK_STOP;
      return true;
    }   
  }
  return false;
}


// Touching function /////////////////////////////////////////////////////////
bool readTouch()
{
  value = ADCTouch.read(A5);   //no second parameter --> 100 samples
  value -= ref;       //remove offset
  if(!TOUCHED && value>=TOUCHTH)
  {
    TOUCHED = true;
    //Serial.println("Touched!!");
  }  
  else if(TOUCHED && value <= TOUCHTH)
  {
    TOUCHED = false;
    //Serial.println("released!!");
  }
  //Serial.println("allora ci siamo o no??");
  return TOUCHED;
}

void resetTouch()
{
  ref = ADCTouch.read(A5, 500); //create reference values to account for the capacitance of the pad
}

