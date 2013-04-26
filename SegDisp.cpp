#include <SegDisp.h>

//constructor
SegDisp::SegDisp(){
	/*
	This is a private parameter, meaning that the user can't call access it directly. It is used to 
	control what digitOn and digitOff due depending on whether or not transistors are used in the circuit
	*/
	tran = true;
	/*
	These are the patterns for the numbers 0-9, sd_EMPTY, and a dash
	C++ does not allow them to be declared directly, so we declare
	them here, then copy them to the actual variable we want.
	
	In order to access them, use SegDisp.numbers[m] where m is the
	number that you want. SegDisp.numbers[sd_EMPTY] is all off and SegDisp.numbers[sd_NEG] is the dash
	*/
	int n[12][7] = {{1,1,1,0,1,1,1,},
	{0,0,1,0,0,0,1,},
 	{0,1,1,1,1,1,0,},
	{0,1,1,1,0,1,1,},
	{1,0,1,1,0,0,1,},
	{1,1,0,1,0,1,1,},
	{1,1,0,1,1,1,1,},
	{0,1,1,0,0,0,1,},
	{1,1,1,1,1,1,1,},
	{1,1,1,1,0,0,1,},
	{0,0,0,0,0,0,0,},
	{0,0,0,1,0,0,0}};
	for (int i=0; i < 12; i++){
		for (int j=0; j<7; j++){
			numbers[i][j] = n[i][j];
		}
	}
	/*
	this is the length of time to delay while multiplexing
	*/
	mpex = 1;
}

//attach the display
void SegDisp::attach(int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int psd_EMPTY, int GND_A, int GND_B, int GND_C, int GND_D){
	/*
	first create an array with all of the pins we need for the patterns
	(these arrays were previously declared)
	*/
	pins[0] = p3;
	pins[1] = p4;
	pins[2] = p5;
	pins[3] = p6;
	pins[4] = p7;
	pins[5] = p8;
	pins[6] = p9;
	pins[7] = psd_EMPTY;
	/*
	this pin controls the colon
	*/
	col = p2;
	/*
	creates an array of pins for the grounds
	*/
	GND[0] = GND_A;
	GND[1] = GND_B;
	GND[2] = GND_C;
	GND[3] = GND_D;
	/*
	format all of the pins as outputs and turn everything off
	*/
	for (int i=0; i<8; i++){
		pinMode(pins[i],OUTPUT);
		digitalWrite(pins[i],LOW);
	}
	for (int i=0; i<4; i++){
		pinMode(GND[i],OUTPUT);
		digitalWrite(GND[i],LOW);
	}
	pinMode(col,OUTPUT);
	digitalWrite(col,LOW);
}

//attach the display using an array
void SegDisp::attach(int pins[13]){
	attach(pins[0],pins[1],pins[2],pins[3],pins[4],pins[5],pins[6],pins[7],pins[8],pins[9],pins[10],pins[11],pins[12]);
}

//write 1 pattern (does not control grounds)
void SegDisp::writePattern(int pattern[7], bool dot){
	/*
	Outputs a single pattern to the seven segments.
	This function does not control the ground pins
	
	The pattern is an array containing HIGH and LOW (or 1 and 0) values
	corresponding to the pins as defined below
	
	*---1---*
	|       |
	0       2
	|       |
	*---3---*
	|       |
	4       6
	|       |
	*---5---*
	*/
	for (int i=0; i<7; i++){
		digitalWrite(pins[i],pattern[i]);
	}
	digitalWrite(pins[7],dot);
}

//make dot optional
void SegDisp::writePattern(int pattern[7]){
	writePattern(pattern[7], false);
}

//make dot optional
void SegDisp::showPattern(int digit, int pattern[7], int ms){
	showPattern(digit,pattern,ms,false);
}

//show 1 pattern
void SegDisp::showPattern(int digit, int pattern[7], int ms, bool dot){
	
	if (ms < 0){
		Error(12);
	}
	else if (digit > 3 || digit < 0){
		Error(13);
	}
	/*
	if we need to include the dot, do so
	Otherwise, just show the pattern
	*/
	writePattern(pattern,dot);
	digitOn(digit);
	delay(ms);
	digitOff(digit);
	digitalWrite(pins[7],LOW); //this is the dot
}

//show 4 patterns for a specified amount of time with dot
void SegDisp::showSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7], int ms, int dot){
	unsigned long start = millis();
	int x=0;
	int p[4][7];
	/*
	Error
	*/
	if (ms<0){
		Error(14);
	}
	else if (dot <-1 || dot > 3){
		Error(15);
	}
	/*
	copy patterns
	*/
	for (int j=0; j<7; j++){
		p[0][j] = pattern1[j];
	}
	for (int j=0; j<7; j++){
		p[1][j] = pattern2[j];
	}
	for (int j=0; j<7; j++){
		p[2][j] = pattern3[j];
	}
	for (int j=0; j<7; j++){
		p[3][j] = pattern4[j];
	}
	/*
	show patterns
	*/
	while (millis()<start+ms){
		showPattern(x,p[x],mpex,(dot==x));
		x = (x+1)%4;
	}
}

//make dot optional
void SegDisp::showSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7], int ms){
	showSequence(pattern1,pattern2,pattern3,pattern4,ms,-1);
}

//show a floating point number for a speficied amount of time
void SegDisp::showFloat(float num, int dec, int ms, char align){
	int d[4] = {sd_EMPTY, sd_EMPTY, sd_EMPTY, sd_EMPTY};
	/*
	Errors
	*/
	if (dec > 3 || dec < 0){
		Error(3);
	}
	else if (num > 9999){
		Error(4);
	}
	else if (num < -999){
		Error(5);
	}
	else if (align!='L' && align!='R'){
		Error(6);
	}
	else if (ms < 0){
		Error(7);
	}
	dec = splitFloat(num,d,dec);
	if (align!='L'){
		dec = dec + moveRight(d);
	}
	showSequence(numbers[d[0]],numbers[d[1]],numbers[d[2]],numbers[d[3]],ms,dec);
}

//align is optional
void SegDisp::showFloat(float num, int dec, int ms){
	showFloat(num,dec,ms,'R');
}

//show integer for a specified amount of time
void SegDisp::showInteger(int num, int ms, char align){
	int d[4] = {sd_EMPTY, sd_EMPTY, sd_EMPTY, sd_EMPTY};
	if (num > 9999){
		Error(8);
	}
	else if (num < -999){
		Error(9);
	}
	else if (align!='L' && align!='R'){
		Error(10);
	}
	else if(ms < 0){
		Error(11);
	}
	splitInteger(num, d);
	
	//handle alignment
	if (align=='L'){
		moveLeft(d);
	}
	
	showSequence(numbers[d[0]],numbers[d[1]],numbers[d[2]],numbers[d[3]],ms,-1);
}

//align is optional (supported)
void SegDisp::showInteger(int num, int ms){
	showInteger(num, ms, 'R');
}

//call this in the setup if transistors were not used
//(reverses on and off)
void SegDisp::noTrans(){
	tran = false;
}

//turn the colon on
void SegDisp::colonOn(){
	digitalWrite(col,HIGH);
} //turn colon on

//turn the colon off
void SegDisp::colonOff(){
	digitalWrite(col,LOW);
} //turn colon off

//turn a digit on
void SegDisp::digitOn(int digit){
	if (digit >3 || digit < 0){
		Error(2);
	}
	// turn on a particular digit (0-4)
	digitalWrite(GND[digit],(tran && HIGH));
}

//turn a digit off
void SegDisp::digitOff(int digit){
	//turn off a particular digit (0-4)
	if (digit >3 || digit < 0){
		Error(1);
	}
	digitalWrite(GND[digit],!(tran && HIGH));
}

//turn on the decimal
void SegDisp::decOn(){
	digitalWrite(pins[7],HIGH);
}

//turn off the decimal
void SegDisp::decOff(){
	digitalWrite(pins[7],LOW);
}

//clear everything
void SegDisp::clear(){
	/*
	clear all of the pins
	*/
	writePattern(numbers[sd_EMPTY],false);
	/*
	turn off the colon
	*/
	colonOff();
	/*
	turn off the dot
	*/
	digitalWrite(pins[7],LOW);
	/*
	disconnect ground
	*/
	for (int x=0; x<4; x++){
		digitOff(x);
	}
}

//make leading optional
void SegDisp::showTime(int a, int b, int ms){
	showTime(a,b,ms,false);
}

//show 2, 2-digit numbers
void SegDisp::showTime(int a, int b, int ms, bool leading){
	int d[4] = {10, 10, 10, 10};
	/*
	show applicable error messages
	*/
	if (a > 99 || a < 0){
		Error(16);
	}
	if (b > 99 || b < 0){
		Error(17);
	}
	if (ms < 0){
		Error(18);
	}
	/*
	turn on the colon
	*/
	colonOn();
	/*
	get the individual digits
	*/
	if (a > 9){
		d[0] = a / 10;
		d[1] = a - d[0]*10;
	}
	else if (leading){
		d[0] = 0;
		d[1] = a;
	}
	else{
		d[1] = a;
	}
	if (b>9){
		d[2] = b / 10;
		d[3] = b - d[2]*10;
	}
	else{
		d[2] = 0;
		d[3] = b;
	}
	/*
	show the sequence
	*/
	showSequence(numbers[d[0]],numbers[d[1]],numbers[d[2]],numbers[d[3]],ms,-1);
	/*
	turn off the colon
	*/
	colonOff();
}
/*

START OF PRIVATE FUNCTIONS

*/

//moves the number all the way to the right
int SegDisp::moveRight(int d[4]){
	bool flag=false;
	int tor = 0;
	/*
	check to see if a real number is present
	otherwise we'll get stuck in an infinite loop
	show the error if necessary
	*/
	for (int x=0; x<4; x++){
		if (d[x]!=sd_NEG && d[x]!=sd_EMPTY){
			flag = true;
		}
	}
	if (!flag){
		Error(1000);
	}
	/*
	move the numbers to the right
	ignore the sd_NEGative sign if there is one
	*/
	while (d[3]==sd_EMPTY){
		d[3] = d[2];
		d[2] = d[1];
		if (d[0] != sd_NEG){
			d[1] = d[0];
			d[0] = sd_EMPTY;
		}
		else{
			d[1] = sd_EMPTY;
		}
		tor ++;
	}
	return tor;
}

//moves the number all the way to the left
int SegDisp::moveLeft(int d[4]){
	int i=0;
	bool flag=false;
	/*
	check to see that at least 1 number is present
	otherwise we're going to get stuck in an infinite loop
	Show the error if necessary
	*/
	for (int x=0; x<4; x++){
		if (d[x]!=sd_NEG && d[x]!=sd_EMPTY){
			flag = true;
		}
	}
	if (!flag){
		Error(1001);
	}
	/*
	move negative numbers to the left, ignoring the negative sign
	*/
	if (d[0] == sd_NEG){
		while(d[1]==sd_EMPTY){
			d[1] = d[2];
			d[2] = d[3];
			d[3] = sd_EMPTY;		
			i++;
		}
	}
	/*
	move positive numbers to the left
	*/
	else{
		while(d[0]==sd_EMPTY){
			d[0] = d[1];
			d[1] = d[2];
			d[2] = d[3];
			d[3] = sd_EMPTY;		
			i++;
		}
	}
	/*
	return the number of places we moved things
	(useful for moving decimal points
	*/
	return i;
}

//make dot optional
void SegDisp::writeSequence(int pattern1[7], int pattern2[7], int pattern3[7], int pattern4[7]){ 
	writeSequence(pattern1,pattern2,pattern3,pattern4,-1);
}

//displays "Err" for a specified amount of time
void SegDisp::Error(int code){
	/*
	create the error patterns
	*/
	int e[7] = {1,1,0,1,1,1,0};
	int r[7] = {0,0,0,1,1,0,0};
	/*
	this holds individual digits when we split the error code
	*/
	int d[4] = {sd_EMPTY,sd_EMPTY,sd_EMPTY,sd_EMPTY};
	/*
	Split the error code into its individual digits, storing the results in d
	*/
	splitInteger(code,d);
	/*
	enter infinite loop showing "Err" and the code
	*/
	while (true){
		showSequence(e,r,r,numbers[sd_EMPTY],1500);
		showSequence(numbers[d[0]],numbers[d[1]],numbers[d[2]],numbers[d[3]],1500);
	}	
}

//split an integer into its digits
void SegDisp::splitInteger(int num, int d[4]){
	int i;
	/*
	reset d to all sd_EMPTY
	*/
	for (i=0; i<4; i++){
		d[i] = sd_EMPTY;
	}
	/*
	we can only display numbers from -999 to 9999 (inclusive)
	show error messages if necessary
	*/
	if (num > 9999){
		Error(1002); //the number is too big
	}
	else if (num < -999){
		Error(1003); //the number is too negative
	}
	else{ //valid number!
		/*
		if the number is negative, put the minus in front
		*/
		if (num < 0){
			d[0] = sd_NEG;
			num = num * -1;
		}
		/*
		if the number is 0, store a zero in the last slot
		*/
		else if (num == 0){
			d[3] = 0;
		}
		/*
		While the number is not zero, strip off the
		last number from the integer and store it in the next available slot
		working right to left
		
		makes use of integer division to make number equal to itself with the last digit stripped off
		*/
		else{
			i=3;
			while (num != 0){
				d[i] = stripLast(num);
				num = num / sd_EMPTY;
				i --;
			}
		}
	}
}

//split a float into its digits, return the decimal location
int SegDisp::splitFloat(float num, int d[4], int dec){
	int base, tor;
	int decimal;
	
	/*
	this is the return value (digit location of decimal)
	*/
	tor = -1;
	
	/*
	empty d
	*/
	for (int i=0; i<4; i++){
		d[i] = sd_EMPTY;
	}
	/*
	make sure dec and num are in the appropriate ranges
	Show error messages if necessary
	*/
	if (dec > 4){
		Error(1004);
	}
	else if (num > 9999){
		Error(1005);
	}
	else if (num < -999){
		Error(1006);
	}
	/*
	no decimal places allocated, treat like integer
	*/
	else if (dec < 1){
		splitInteger((int)num, d);
	}
	/*
	This is really a floating point number
	*/
	else{
		/*
		base is the part before the decimal
		*/
		base = floor(num);
		/*
		this should be 0.something
		*/
		num = num-base;
		/*
		no decimal place will fit, treat it like an integer
		but show a decimal place at the end
		*/
		if (base > 999 || base < -99){
			splitInteger(base, d);
			tor = 3;
		}
		/*
		the decimal place fits, so keep going
		*/
		else{
			int i = 3;
			/*
			starting at the right
			if the base is 0, put it in
			if it's negative, put in the negative sign
			*/
			if (base==0){
				d[i] = 0;
			}
			else if (base < 0){
				d[0] = sd_NEG;
				base = base * -1;
			}
			/*
			working from right to left, we strip off the
			last number, and store it in the proper place
			*/
			while (base != 0){
				d[i] = stripLast(base);
				base = base / 10;
				i--;
			}
			/*
			now, move the base all the way to the left
			the decimal goes at the end of the base
			*/
			tor = 3 - moveLeft(d);
			/*
			we want to start writing the decimal one place
			to the right of where we just ended
			*/
			i = tor+1;
			/*
			if there are still places to right numbers
			(there should be)
			*/
			if (i < 4){
				int dec_left;
				if (dec < (4-i)){
					dec_left = dec;
				}
				else{
					dec_left = 4-i;
				}
				/*
				not all numbers are stored perfectly using binary. Therefore
				the easy method of conversion doesn't work.
				
				Instead, we take the decimal place and convert it into an integer
				with the same number of digits as we have decimal places.
				Then, if the decimal remainder >= .9, we round up. Otherwise,
				round down
				*/
				if (num*pow(10,dec_left) - floor(num*pow(10,dec_left)) >= 0.5){
					decimal = ceil(num * pow(10,dec_left));
				}
				else{
					decimal = floor(num * pow(10,dec_left));
				}
				/*
				while there are still numbers to fill and we 
				have not maxed out the number of decimal places
				*/
				i = tor + dec_left;
				while (i > tor){
					d[i] = stripLast(decimal);
					decimal = decimal / 10;
					i--;
				}
			}
			/*
			something went wrong, not sure what
			*/
			else{
				Error(1007);
			}
		}
	}
	return tor;
}

//return the first digit of an integer
int SegDisp::stripFirst(int n){
	int x = 1;
	/*
	cannot process negative number
	*/
	if (n<0){
		Error(1008);
	}
	/*
	get the first digit
	*/
	while (x <= n){
		x = x * 10;
	}
	return n * 10 / x;
}

//return the last digit of an integer
int SegDisp::stripLast(int n){
	/*
	cannot process negative numbers
	*/
	if (n<0){
		Error(1009);
	}
	return n%10;
}