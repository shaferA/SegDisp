//include the library
#include <SegDisp.h>

//create a new display
SegDisp myseg = SegDisp();

/*
These are the custom patterns
1 for ON, 0 for OFF
follows this pattern:

*-2-*
1   3
*-4-*
5   7
*-6-*
*/
int a[7] = {1,0,0,0,1,0,0};
int b[7] = {0,0,1,0,0,0,1};
int c[7] = {1,0,1,0,1,0,1};

void setup(){
  //if wired as shown in the tutorial, these can stay the same
  //otherwise, change these pins to match yours
  myseg.attach(2,3,4,5,6,7,8,9,10,11,12,13,A5);  
}

void loop(){
  /*
  creates the following pattern:
  |
  ||
  |||
  etc.
  
  showSequence displays 4 patterns (1 per digit) for a set amount of time
  an optional additional parameter will display the dot on the specified digit
  
  You can use custom or built-in patterns
  The built-in patterns are accessed via: myseg.numbers[X]
  
  where X is 0-9 for the numbers 0-9
  X=sd_EMPTY for a blank digit
  X=sd_NEG for a dash
  */
  myseg.showSequence(a,myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,a,myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,c,myseg.numbers[sd_EMPTY],myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,c,a,myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,c,c,myseg.numbers[sd_EMPTY],500);
  myseg.showSequence(c,c,c,a,500);
  myseg.showSequence(c,c,c,c,500);
}
