/*
 * main.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: angie
 */


#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cpu.h"
#include "memory.h"
#include"message.h"
using namespace std;

int main(int argc, char* argv[])
{
     int p1[2];
   int p2[2];
   int result;
   result = pipe(p1);
   if (result == -1)
        exit(1);
   result = pipe(p2);
   if (result == -1)
        exit(1);
   result = fork();
   if (result == -1)
        exit(1);
   if (result == 0)     //memory process
   {
	   memory mmr;
	   mmr.readFile(argv[1]);
	   mmr.setPipeMemory(p1[0],p2[1]);
	   while(1)
	   {
		   mmr.memoryRead();
		   mmr.memoryCheckmsg();
		   mmr.memoryWrite(mmr.m.msg);
	   }
   }
   else        //cpu process
   {
	   cpu c;
       c.setPipeCPU(p1[1],p2[0]);
       c.setTimerCount((atoi)(argv[2]));
       c.cpuExecute();

       waitpid(-1, NULL, 0);
   }
   return 0;
}

