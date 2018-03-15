import processing.serial.*;

Serial myPort;
final int ADDR_1 = 15;
final int ADDR_2 = 16;
int addr = 0;
int count = 0;
int data = 0;

// SETUP ///////////////////////////////////////////////
void setup()
{
  size(200, 200);
  //printArray(Serial.list());
  String port = Serial.list()[3];
  myPort = new Serial(this, port, 9600); 
}

// LOOP ////////////////////////////////////////////////
void draw()
{
  //do nothing
}

// KEY PRESSED /////////////////////////////////////////
void keyPressed()
{
  if(key == ' ')
    myPort.write('s');  
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
    print("ch ");print(addr); print(": ");
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
        print(data); println(";");
      }
    }
    else if(addr == ADDR_2)
    {
      // data for ADDR_2 is only 1 byte long
      data = c;
      print(data); println(";");    
    }
  }
}