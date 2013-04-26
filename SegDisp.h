/*
7 segment display library
March 28 2013
By: Austin Shafer

Creative Commons License
*/

#ifndef SegDisp_H
#define SegDisp_H

#include <Arduino.h>

#define sd_NEG 11
#define sd_EMPTY 10
class SegDisp {
public:
	//constructor
	SegDisp();
	//attach the 7 segment display,
	void attach(int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int GND_A, int GND_B, int GND_C, int GND_D); 
	//attach the 7 segment display using an array
	void attach(int pins[13]);
	//show 1 pattern (only handles pins, not ground)
	void writePattern(int pattern[7], bool dot);
	//make dot optional
	void writePattern(int pattern[7]);
	//display 1 pattern for ms milliseconds w/ dot
	void showPattern(int digit, int pattern[7], int ms, bool dot);
	//display 1 pattern for ms milliseconds w/o dot
	void showPattern(int digit, int pattern[7], int ms);
	//display 4 patterns for ms milliseconds w/ dot
	void showSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7], int ms, int dot);
	//display 4 patterns for ms milliseconds w/o dot
	void showSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7], int ms);
	//show a float for ms milliseconds, aligned using align
	void showFloat(float num, int dec, int ms, char align);
	//make align optional
	void showFloat(float num, int dec, int ms);
	//show an integer for ms milliseconds, aligned using align
	void showInteger(int num, int ms, char align);
	//make align optional
	void showInteger(int num, int ms);
	//call this if not working with transistors
	void noTrans();
	//turn colon on
	void colonOn();
	//turn colon off
	void colonOff();
	//turn a digit off
	void digitOff(int digit);
	//turn a digit on
	void digitOn(int digit);
	//turn the decimal on
	void decOn();
	//turn the decimal off
	void decOff();
	//clear the display (all ground/pin low)
	void clear();
	//shows 2, 2-digit numbers
	void showTime(int a, int b, int ms);
	//make leading optional
	void showTime(int a, int b, int ms, bool leading);
	//these are the number patterns. Note that no decimal point access is provided
	int numbers[12][7];
private:
	//move the numbers right
	int moveRight(int d[4]);
	//move the numbers left
	int moveLeft(int d[4]);
	//show 4 patterns at once 
	void writeSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7], int dot); //write a sequence of 4 patterns
	//make dot optional
	void writeSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7]);
	//show "Err" with code
	void Error(int code);
	//split an integer into its individual digits
	void splitInteger(int num, int d[4]);
	//split a float into its individual digits
	int splitFloat(float num, int d[4], int dec);
	//get the first digit
	int stripFirst(int n);
	//get the last digit
	int stripLast(int n);
	//input pins
	int pins[8];
	//pin for the colon
	int col;
	//ground pins
	int	GND[4];
	//whether or not transistors were used in the setup
	bool tran; 
	//length of time to delay while multiplexing
	int mpex;
};
#endif