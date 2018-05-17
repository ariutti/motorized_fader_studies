/*
 * A sketch to be used with the QT113
 * touch sensing chip.
 */
#define TOUCH 2
#define LED 13

void setup() 
{
  Serial.begin(9600);
  pinMode(TOUCH, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() 
{
  digitalWrite(LED, !digitalRead(TOUCH));
  delay(100);
}
