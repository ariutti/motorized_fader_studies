/*
 * A simple sketch to work with a NEMA14 +
 * a Keyes stepper motor board + a RepRap StepStick +
 * 2x push buttons + 2x 10 KOhm resistors.
 *
 * Look at mages to see how the circuit is made.
 */


// Motor Stuff
#define E 11
#define D 10
#define S 9
bool enable = false;
bool dir    = false;
bool step   = false;
unsigned long currTime, lastTime = 0.0;

// Buttons stuff
#define ENABLE 3
#define DIRECTION 2


// SETUP /////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

  // Motor stuff
  pinMode(E, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(S, OUTPUT);
  digitalWrite(E, !enable); // A4988 pin has an inverse logic
  digitalWrite(D, dir);
  digitalWrite(S, step);
  currTime, lastTime = micros();

  // Button stuff
  pinMode(ENABLE, INPUT);
  pinMode(DIRECTION, INPUT);
}



// LOOP //////////////////////////////////////////////////
void loop()
{
  // Read the states of the buttons.
  // ENABLE BUTTON:
  // if enable button is pressed and now the motor di disable
  // then turn it on.
  if(digitalRead(ENABLE) && !enable)
  {
    enable = true;
    digitalWrite(D, dir);
    digitalWrite(E, !enable);
  }
  // else if the motor is running and I've released
  // the button, then turn the motor off
  else if(!digitalRead(ENABLE) && enable)
  {
    enable = false;
    digitalWrite(D, dir);
    digitalWrite(E, !enable);
  }


  // DIRECTION BUTTON:
  // everytime I press the button, the direction
  // wil change.
  if(digitalRead(DIRECTION) && !dir)
  {
    dir = true;
    digitalWrite(D, dir);
  }
  else if(digitalRead(DIRECTION) && dir)
  {
    dir = false;
    digitalWrite(D, dir);
  }


  currTime = micros();
  if( (currTime - lastTime) > 100)
  {
    step = !step;
    digitalWrite(S, step);
    //Serial.println(step);
    lastTime = currTime;
  }

}
