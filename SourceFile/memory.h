/*
 * memory.h
 *
 *  Created on: Feb 19, 2016
 *      Author: angie
 */

#include <stdlib.h>
#include <unistd.h>
#include"message.h"
//using namespace std;

class memory {
public:
	int arr[2000];
	int n;
	int p1;
	int p2;
	message m;
	void memoryWrite(char c);
	void memoryRead();
	void setPipeMemory(int psd1,int psd2);
	int read1(int address);
	void write1(int address, int val);
	int readFile(char* p);
	void memoryCheckmsg();

};


