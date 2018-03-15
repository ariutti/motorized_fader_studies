import processing.serial.*;
Serial myPort;
int addr, expect, count;
int type;
final int TOUCH = 1;
final int POT = 2;
int value;

void setup()
{
  size(400,200);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[3], 9600); 
  value = 0;
}

void draw()
{
  delay(10);
}

void serialEvent(Serial s)
{
  int c = s.read();
  if(c>127)
  {
    // this is a staut byte
    type = addr;
    count = 0;
  }
  else
  {
    // data bytes
    if(type==POT)
    {
      if(count == 0)
      {
      value = (c & 0x07F)<<7;
      count ++;
      }
      else if(count == 1)
      {
        value = value | (c & 0x07F);
        println(value);
      }
    }
    else if(type==TOUCH)
    {
      value = (c & 0x07F);
      println(value);
    }
  }
  
}