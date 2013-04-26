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
  myseg.showFloat(2,2,1000);
   myseg.showFloat(2.345,2,1000);
   myseg.showFloat(0.214,3,1000);
   myseg.showFloat(24.123,0,1000);
   myseg.showFloat(0,2,1000,'L');
  
}
