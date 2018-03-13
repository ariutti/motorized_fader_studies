/* 
 *  test skectch using the ACDTouch library.
 */

#include <ADCTouch.h>

int ref;     //reference values to remove offset
long int t;

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings
    Serial.begin(9600);

    ref = ADCTouch.read(A5, 500);    //create reference values to account for the capacitance of the pad
    t = millis();  
} 

void loop() 
{
  Serial.print(millis() - t);
  t = millis();
  int value = ADCTouch.read(A5);   //no second parameter --> 100 samples
  value -= ref;       //remove offset
  Serial.print("\t");
  Serial.println(value);
  //delay(100);
}
