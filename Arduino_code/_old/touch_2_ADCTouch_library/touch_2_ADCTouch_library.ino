/* 
 *  test skectch using the ACDTouch library.
 */

#include <ADCTouch.h>

float ref;     //reference values to remove offset
long int t;

// filter stuff
float B, A = 0.125;
float value;

// SETUP ///////////////////////////////////////////////
void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings
    Serial.begin(9600);

    ref = ADCTouch.read(A5, 500);    //create reference values to account for the capacitance of the pad
    t = millis();  

    // filter stuff
    B = 1.0 - A;
} 


// LOOP ////////////////////////////////////////////////
void loop() 
{
  Serial.print(millis() - t);
  t = millis();
  float data = ADCTouch.read(A5, 5);   //if no second parameter --> 100 samples
  value = data*A + B*value;
  //value -= ref;       //remove offset
  Serial.print("\t");
  Serial.println((int)value);
  //delay(100);
}


