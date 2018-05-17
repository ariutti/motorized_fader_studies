// make the following connections
// QT113 pin 1 - to Arduino pin 12 (put a filtering 10nF C);
// QT113 pin 2 - to Arduino pin 8 (touch sensing);
// QT113 pin 3 - to Vcc;
// QT113 pin 4 - to GND;
// QT113 pin 5 - to GND;
// QT113 pin 6 - to the negative side of a 10nF C
// QT113 pin 7 - to the other side of the 10nF C and to the electrode
// QT113 pin 8 - to GND

#define LED 13
#define TOUCH 8
#define QT113 12

unsigned long elapsed;
unsigned long del = 10000;

// SETUP ////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  
  pinMode(TOUCH, INPUT);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  pinMode(QT113, OUTPUT);
  digitalWrite(QT113, HIGH); 
   
  elapsed = millis();
}

// LOOP /////////////////////////////////////////////////////////////
void loop() 
{
  int value = digitalRead(TOUCH);
  digitalWrite(LED, value);
  //Serial.println(value);

  /*
  if(millis() - elapsed > del)
  {
    elapsed = millis();
    digitalWrite(QT113, LOW);
    digitalWrite(QT113, HIGH);
    Serial.println("QT113 reset");
  }  
  */
  delay(100);
}
