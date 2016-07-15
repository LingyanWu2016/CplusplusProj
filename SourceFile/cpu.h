/*
 * cpu.h
 *
 *  Created on: Feb 19, 2016
 *      Author: angie
 */



#include <stdlib.h>
#include<math.h>
#include <unistd.h>
#include"message.h"
using namespace std;



class cpu
{
public:
	int p3;
    int p4;
    int val;
    int COUNT;
	int PC, SP, IR, AC, X, Y;
	message m;
	bool system_mode;
	void cpuWrite(int addr,char c);
	int cpuRead();
	void setPipeCPU(int psd1,int psd2);
	void setTimerCount(int count);
	void cpuExecute();
	void load (int val);
	void get_AC();
	void put_port(int port);
	int jmp_addr(int val);
	int jmpif(int address);
	int jmpifnot(int address);
	void incX();
	void decX();
	void Int(bool flag);
	void end();
};



