/*
 * This sketch is a little modification of the example 
 * Taken directly from the Pololu library.
 */

/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

float A = 0.7;
float B;
float prevValue = 0.0, value = 0.0;


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // calculate corefficients for the LPF filter
  B = 1.0 - A;

  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}


void loop()
{
  value = sensor.readRangeContinuousMillimeters();
  
  // apply a low pass filtering
  value = A * value + B * prevValue;
  Serial.print(value);
  prevValue = value;
  
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}
