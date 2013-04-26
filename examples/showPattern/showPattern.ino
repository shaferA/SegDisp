//include the library
#include <SegDisp.h>

//create a new display
SegDisp myseg = SegDisp();

int x=0;

void setup(){
  //if wired as shown in the tutorial, these can stay the same
  //otherwise, change these pins to match yours
  myseg.attach(2,3,4,5,6,7,8,9,10,11,12,13,A5); 
  
}

void loop(){
  //loop through all digits
  x=(x+1)%4;
  //write the pattern (no dot), wait 500 ms
  //note: the last parameter is optional, defaults to false
  myseg.showPattern(x,myseg.numbers[x],500,false);
  
}
