/*
* Limulo_Motor.h - a library to use the
* Sparkfun Moto Shield: https://www.sparkfun.com/products/10182
* Most part of the code is inspired from the code found in the
* official GitHub repo: https://github.com/sparkfun/Monster_Moto_Shield/tree/Hv12Fv10
* Created by Nicola Ariutti, March 09, 2018
* TODO: license
*/
#ifndef _LIMULO_MOTOR
#define _LIMULO_MOTOR
#include "Arduino.h"

class Motor
{
public:
	Motor() {};

	void init();
	void enable(uint8_t motor);
	void disable(uint8_t motor);
	void stop();
	void forward();
	void reverse();
	void increaseSpeed();
	void decreaseSpeed();
	void setSpeed(uint8_t speed);

	uint16_t getCurrent(uint8_t motor);
	uint8_t getSpeed();

	void printCurrents();

	// 14/03/2018 - utility function to make the motor go slow
	// This is a "dirty" approach to create a sort
	// of custom PWM that will enable/disable the motor.
	//void updatePWM(uint8_t _motor);


private:

	void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm);

	enum {
		BRAKE = 0,
		CW,
		CCW,
	} state = BRAKE;

	// Definition of safety current (Check: "1.3 Monster Shield Example").
	int CS_THRESHOLD = 20;

	unsigned short speed = 15;  //default motor speed

	//MOTOR 1
	bool MOTOR_1_ENABLED = false;
	const int MOTOR_1 = 0;
	const int EN_PIN_1 = A0;
	const int MOTOR_A1_PIN = 7;
	const int MOTOR_B1_PIN = 8;
	const int PWM_MOTOR_1 = 5;
	const int CURRENT_SEN_1 = A2;

	//MOTOR 2
	bool MOTOR_2_ENABLED = false;
	const int MOTOR_2 = 1;
	const int EN_PIN_2 = A1;
	const int MOTOR_A2_PIN = 4;
	const int MOTOR_B2_PIN = 9;
	const int PWM_MOTOR_2 = 6;
	const int CURRENT_SEN_2 = A3;

	// 14/03/2018 - utility variables to make the motor go slow
	// This is a "dirty" approach to create a sort
	// of custom PWM that will enable/disable the motor.
	uint16_t period, halfPeriod;
	long duration, st;

};
#endif
