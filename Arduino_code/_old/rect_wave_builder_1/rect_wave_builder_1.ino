
int period = 500; //ms
int halfPeriod;
long st;
int state = 0;





void setup()
{
  Serial.begin(115200);
  halfPeriod = period / 2;
  st = millis();
}



void loop() 
{
  long duration = micros()-st;
  if(duration<halfPeriod)
  {
    state = 1;
  }
  else if(duration<period)
  {
    state = 0;
  }
  else
  {
    st = micros();
  }
  Serial.println(state);
}


