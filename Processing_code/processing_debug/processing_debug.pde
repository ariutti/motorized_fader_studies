import processing.serial.*;
Serial myPort;
String myString = null;
String[] list;
int lfcr = 10;
boolean bSerialListen = false;

// SETUP ////////////////////////////////
void setup() {
  size(100, 100);
  smooth();
  
  // serial stuff
  printArray(Serial.list());
  String port = Serial.list()[3];
  myPort = new Serial(this, port, 9600); 
}

// DRAW /////////////////////////////////
void draw() {
  background(0);
  read();
}


// READ values from Serial //////////////
void read()
{
  while (myPort.available() > 0) {
    myString = myPort.readStringUntil( lfcr );
    if (myString != null) {
      list = split(myString, ',');
      
      if( bSerialListen )
      {
        print(myString);
        printArray(list);
        println(list.length);
      }
      
      /*
      for(int i=0; i<list.length-1; i++) {
        tP[i] = int(list[i]);
      }
      */
      
      // print all the converted strings
      /*
      print("[ ");
      for(int i=0; i<16; i++)
      {
        print(tP[i]); 
        if(i!=15) print(", ");
      }
      print(" ]\n");
      */
    }
  }
}


// KEY PRESSED //////////////////////////////////////////////////////////////////////
void keyPressed()
{
  if (key == 'o' || key == 'O')
  {
    myPort.write('o');
    bSerialListen = true;
  }
  else if( key == 'c' || key == 'C')
  {
    myPort.write('c');
    bSerialListen = false;
  }
  else if (key == 'r' || key == 'R')
  {
    myPort.write('r');
  }
}
