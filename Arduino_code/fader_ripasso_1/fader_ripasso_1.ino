/* TODO: Documentation HERE
 * TODO: comment unnecessary debug messagescome procede
 * 
 */

// Master State Machine ******************/
enum {
  MASTER_TOUCH = 0,
  MASTER_WAIT4STANDBY,
  MASTER_SEEK
} master_state, prev_master_state;


// Motor / Potentiometer stuff ***********/
#include <Limulo_Motor.h>
Motor motor;

#define POT A4
int ADDR_POT = 15;
uint16_t pot, destination;
int STOP_LOW = 20;
int STOP_HIGH = 1000;
int MOTORSPEED = 120;
int DLY_TIME = 2;
int DISTANCE = 400;
// TODO: add current threshold limiter

enum {
  SEEK_QUIET = 0,
  SEEK_FORWARD,
  SEEK_REVERSE
} seek_state, prev_seek_state;

// Touch stuff ***************************/
#define TOUCH 2
#define LED 13
bool prevTOUCHED, TOUCHED = false;

/*
#include <ADCTouch.h>
int ADDR_TOUCH = 16;
int ref; //reference values to remove offset
const int TH_TOUCH = 800;
const int TOUCH_READINGS = 10;
bool TOUCHED = false;
// filter touching
float B, A = 0.125;
float tmp, value;
*/

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  Serial.begin(115200);  // Initiates the serial to do the monitoring 
  /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  */

  // touch stuff
  //resetTouch();
  pinMode(TOUCH, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  // motor stuff
  motor.init(10000); //(10ms is the best tradeoff)
  motor.enable(1);
  motor.disable(2);
  motor.setSpeed(MOTORSPEED);
  motor.stop();  
  master_state = prev_master_state = MASTER_SEEK;
  seek_state = prev_seek_state = SEEK_QUIET;
  chooseNewDestination();

  // filter stuff
  //B = 1.0 - A;
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  
  // reply only when you receive data:
  if (Serial.available() > 0) 
  {
    manageSerial();
  }

  pot = analogRead(POT);

  printStates();

  /*
  Serial.print('p');
  Serial.print(pot);
  Serial.print(';'); 
  Serial.println();
  */
  
  // Master state machine //////////////////////////
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

    motor.updatePWM(1);    

    if( isArrived() )
    {
      // we are almost arrived to the destination
      // stop the motor and change state
      Serial.println("Destination reached!");
      motor.stop();
      seek_state = SEEK_QUIET;
      master_state = MASTER_WAIT4STANDBY;
      // this code is executed only when we reach
      // the destination we selected.
      //seek_startWaitingTime = millis();
    } 
    
  } // end of MASTER_SEEK state

  delay(DLY_TIME);
}



// SERIAL PRINT //////////////////////////////////////////////////////////////
void manageSerial()
{
  char user_input = Serial.read(); // Read user input and trigger appropriate function
    
  if(user_input =='+')  {
    motor.increaseSpeed();
  }
  else if(user_input =='-')  {
    motor.decreaseSpeed();
  }
  else if(user_input == 's' )
  {
    Serial.println("\nYou have pressed S");
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

// Seeking functions /////////////////////////////////////////////////////////
void chooseNewDestination()
{
  Serial.println("choose new desitnation");
  pot = analogRead(POT);
  
  destination = getTarget();
  int distance = destination - pot;

  if(distance > 0)
  {
    Serial.println("going Forward");
    motor.forward();
    seek_state = SEEK_FORWARD;
  }
  else if(distance < 0)
  {
    Serial.println("going reverse");
    motor.reverse();
    seek_state = SEEK_REVERSE;
  }
  else
  {
    chooseNewDestination();
  }
}

// A function to always select a destination which is
// far away fron the current potentiometer position.
int getTarget()
{
  Serial.println("getTarget");
  int t = random(STOP_LOW, STOP_HIGH);
  int d = t - pot;
  if( abs(d) > DISTANCE )
  {
    return t;
  }
  else
  {
    return getTarget();
  }
}

bool isArrived()
{
  if(seek_state == SEEK_FORWARD ) // sub-state machine (seeking)
  {
    //Serial.println(pot);
    if(pot >= destination)
    {
      return true;
    }
  } 
  else if(seek_state == SEEK_REVERSE )
  {
    //Serial.println(pot);
    if(pot <=destination)
    {
      return true;
    }   
  }
  return false;
}

// Touching function ////////////////////////////////////////////////////////
bool readTouch()
{
  TOUCHED = !digitalRead(TOUCH);
  if(!prevTOUCHED && TOUCHED)
  {
    digitalWrite(LED, TOUCHED);
    Serial.print('t');
    Serial.print(1);
    Serial.print(';');
    prevTOUCHED = TOUCHED;

  }  
  else if(prevTOUCHED && !TOUCHED)
  {
    digitalWrite(LED, TOUCHED);
    Serial.print('t');
    Serial.print(0);
    Serial.print(';');
    prevTOUCHED = TOUCHED;
  }
  //Serial.println("allora ci siamo o no??");
  return TOUCHED;
}


// DEBUG functions //////////////////////////////////////////////////////////
void printStates()
{
  if(master_state != prev_master_state)
  {
    switch(master_state)
    {
      case 0:
        Serial.println("MASTER_TOUCH");
        break;
      case 1:
        Serial.println("MASTER_WAIT4STANDY");
        break;
      case 2:
        Serial.println("MASTER_SEEK");
        break;
      default:
        break;
    }
  prev_master_state = master_state;
  }

  if(seek_state != prev_seek_state)
  {
    switch(seek_state)
    {
      case 0:
        Serial.println("\tSEEK_QUIET");
        break;
      case 1:
        Serial.println("\tSEEK_FORWARD");
        break;
      case 2:
        Serial.println("\tSEEK_REVERSE");
        break;
      default:
        break;
    } 
    prev_seek_state = seek_state;
  }
}
