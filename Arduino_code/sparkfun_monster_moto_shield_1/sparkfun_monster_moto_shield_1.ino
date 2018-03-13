/*
 * The shield that we are usign is the sparkfun monster moto shield
 * See the following link: https://www.sparkfun.com/products/10182 
 * This sketch is taken from this website: http://www.instructables.com/id/Monster-Motor-Shield-VNH2SP30/
 */

#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_1 0
#define EN_PIN_1 A0
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
#define PWM_MOTOR_1 5
#define CURRENT_SEN_1 A2

//MOTOR 2
#define MOTOR_2 1
#define EN_PIN_2 A1
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9
#define PWM_MOTOR_2 6
#define CURRENT_SEN_2 A3

short usSpeed = 25;  //default motor speed
unsigned short usMotor_Status = BRAKE;

// SETUP /////////////////////////////////////////////////////////////////////
void setup()                         
{
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  //pinMode(CURRENT_SEN_1, OUTPUT);
  //pinMode(CURRENT_SEN_2, OUTPUT);  

  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  Serial.begin(9600);  // Initiates the serial to do the monitoring 
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. STOP");
  Serial.println("2. FORWARD");
  Serial.println("3. REVERSE");
  Serial.println("4. READ CURRENT");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();
}


// LOOP //////////////////////////////////////////////////////////////////////
void loop() 
{
  char user_input;   

  while( Serial.available() )
  {
    user_input = Serial.read(); // Read user input and trigger appropriate function
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(EN_PIN_2, LOW); 
     
    if (user_input =='1')
    {
       Stop();
       printCurrents();
    }
    else if(user_input =='2')
    {
      Forward();
      printCurrents();
    }
    else if(user_input =='3')
    {
      Reverse();
      printCurrents();
    }
    else if(user_input == '4')
    {
      printCurrents();      
    }
    else if(user_input =='+')
    {
      IncreaseSpeed();
      printCurrents();
    }
    else if(user_input =='-')
    {
      DecreaseSpeed();
      printCurrents();
    }
    else
    {
      Serial.println("Invalid option entered.");
    }
  }
}


// CUSTOM FUNCTIONS //////////////////////////////////////////////////////////
void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 5;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 5;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

//Function that controls the variables: motor(0 or 1), direction (cw ou ccw) e pwm (enter between 0 and 255);
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);            
    }
    analogWrite(PWM_MOTOR_2, pwm);
  }
}

void printCurrents()
{
  int v1 = analogRead(CURRENT_SEN_1);
  int v2 = analogRead(CURRENT_SEN_2);
  Serial.print( v1 ); Serial.print("\t");
  Serial.print( v2 ); Serial.print("\n");
}


