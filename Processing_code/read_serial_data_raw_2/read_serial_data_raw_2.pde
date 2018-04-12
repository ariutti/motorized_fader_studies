// serial stuff
import processing.serial.*;
Serial myPort;
final int ADDR_1 = 15;
int addr = 0;
int count = 0;
int data = 0;
float h = 0.0;

// SETUP ///////////////////////////////////////////////
void setup()
{
  size(200, 200);
  printArray(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 9600);
  //rectMode(CORNERS);
}

// LOOP ////////////////////////////////////////////////
void draw()
{
  background(255);
  
  pushStyle();
  noStroke();
  fill(255, 0, 0);
  rect(0, height, width, -h);
  popStyle();
}

// SERIAL EVENT ////////////////////////////////////////
void serialEvent(Serial s)
{
  int c = s.read();
  //println(c);
  if(c>127)
  {
    // status byte
    addr = c & 0x7F;
    //print("ch ");print(addr); print(": ");
    count = 0;
  }
  else
  {
    // data byte
    if(addr == ADDR_1)
    {
      // data for ADDR_1 are splitted into two bytes
      if(count == 0)
      {
        data = (c & 0x07) << 7;
        count++;
      }
      else if(count == 1)
      {
        data = data | (c & 0x7F);
        //print(data); println(";");
        h = map(data, 0, 1023, 0, height);
      }
    }
  }
}