//include the library
#include <SegDisp.h>

//create a new display
SegDisp myseg = SegDisp();

void setup(){
  //if wired as shown in the tutorial, these can stay the same
  //otherwise, change these pins to match yours
  myseg.attach(2,3,4,5,6,7,8,9,10,11,12,13,A5);  
}

void loop(){
  //parameters from left to right are:
  //integer to display
  //time to display it (ms)
  //alignment (L or R)
  myseg.showInteger(1,1000,'L');
  myseg.showInteger(12,1000,'L');
  myseg.showInteger(123,1000,'L');
  myseg.showInteger(1234,1000,'L');
  myseg.showInteger(234,1000,'R');
  myseg.showInteger(34,1000); //alignment is optional
  myseg.showInteger(4,1000); //defaults to R
  
  //clear the display and wait 1 second
  myseg.clear();
  delay(1000);
}



