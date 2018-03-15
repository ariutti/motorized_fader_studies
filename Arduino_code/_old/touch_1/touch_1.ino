#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

// Send Pin - Receive Pin
CapacitiveSensor   cs_7_8 = CapacitiveSensor(7,8); // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
long start, reading;

void setup()                    
{
   cs_7_8.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
}

void loop()                    
{
    start = millis();
    reading =  cs_7_8.capacitiveSensor(2);

    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing
    Serial.print(reading);                  // print sensor output 
    Serial.print("\n"); 
    Serial.println("hello");
    delay(10);                             // arbitrary delay to limit data to serial port 
}
