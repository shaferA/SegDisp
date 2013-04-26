//include the library
#include <SegDisp.h>

//create a new display
SegDisp myseg = SegDisp();

int p[7] = {1,1,1,1,1,1,1};
int x=0;

void setup(){
  //if wired as shown in the tutorial, these can stay the same
  //otherwise, change these pins to match yours
  myseg.attach(2,3,4,5,6,7,8,9,10,11,12,13,A5); 
  
  //turn all digits on
  for (int i=0; i<4; i++){
    myseg.digitOn(0);
  }
}

void loop(){
  //loop through all preset patterns
  x=(x+1)%12;
  //write the pattern (no dot)
  myseg.writePattern(myseg.numbers[x],false);
  //wait 300 milliseconds
  delay(300);
}
