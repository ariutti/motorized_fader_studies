#include "Limulo_Motor.h"

void Motor::init(uint16_t _period)
{
	// setup for motor 1
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);

  // setup for motor 2
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  // initilize motors to brake
  stop();

	// 14/03/2018 - utility variable initialization
	// for the custom PWM for the motor to go slower to
	// be made.
	period = _period;
	halfPeriod = period / 2;
  st = micros();
}

void Motor::enable(uint8_t _motor)
{
	if(_motor == 1)
	{
		MOTOR_1_ENABLED = true;
		digitalWrite(EN_PIN_1, HIGH);
	}
	else if(_motor == 2)
	{
		MOTOR_2_ENABLED = true;
		digitalWrite(EN_PIN_2, HIGH);
	}
}

void Motor::disable(uint8_t _motor)
{
	if(_motor == 1)
	{
		MOTOR_1_ENABLED = false;
		digitalWrite(EN_PIN_1, LOW);
	}
	else if(_motor == 2)
	{
		MOTOR_2_ENABLED = false;
		digitalWrite(EN_PIN_2, LOW);
	}
}

uint16_t Motor::getCurrent(uint8_t motor)
{
	if(motor == 1)
		return analogRead(CURRENT_SEN_1);
	else if(motor == 2)
		return analogRead(CURRENT_SEN_2);
}

void Motor::setSpeed(uint8_t _speed)
{
	speed = _speed;
}

uint8_t Motor::getSpeed()
{
	return speed;
}

// Function that controls the variables:
// * motor(0 or 1);
// * direction (cw ou ccw);
// * pwm (enter between 0 and 255);
void Motor::motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
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
			// stop the motors
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
			// stop the motors
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
    }
    analogWrite(PWM_MOTOR_2, pwm);
  }
}

void Motor::printCurrents()
{
  int v1 = analogRead(CURRENT_SEN_1);
  int v2 = analogRead(CURRENT_SEN_2);
  Serial.print( v1 ); Serial.print("\t");
  Serial.print( v2 ); Serial.print("\n");
}

// CUSTOM FUNCTIONS //////////////////////////////////////////////////////////

// 14/03/2018 - utility function to make the motor go slow
// This is a "dirty" approach to create a sort
// of custom PWM that will enable/disable the motor.
void Motor::updatePWM(uint8_t _motor)
{
	duration = micros()-st;
	//Serial.print("uPWM: ");
	//Serial.println(duration);
  if(duration<halfPeriod)
  {
    state = 1;
		enable(_motor);
  }
  else if(duration<period)
  {
    state = 0;
		disable(_motor);
  }
  else
  {
    st = micros();
  }
}

void Motor::stop()
{
  //Serial.println("Stop");
  state = BRAKE;
  motorGo(MOTOR_1, state, 0);
  motorGo(MOTOR_2, state, 0);
}

void Motor::forward()
{
  //Serial.println("Forward");
  state = CW;
  motorGo(MOTOR_1, state, speed);
  motorGo(MOTOR_2, state, speed);
}

void Motor::reverse()
{
  //Serial.println("Reverse");
  state = CCW;
  motorGo(MOTOR_1, state, speed);
  motorGo(MOTOR_2, state, speed);
}

void Motor::increaseSpeed()
{
  speed = speed + 5;
  if(speed > 255)
  {
    speed = 255;
  }

  //Serial.print("Speed +: ");
  //Serial.println(speed);

  motorGo(MOTOR_1, state, speed);
  motorGo(MOTOR_2, state, speed);
}

void Motor::decreaseSpeed()
{
  speed = speed - 5;
  if(speed < 0)
  {
    speed = 0;
  }

  //Serial.print("Speed -: ");
  //Serial.println(speed);

  motorGo(MOTOR_1, state, speed);
  motorGo(MOTOR_2, state, speed);
}
