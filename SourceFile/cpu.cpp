/*
 * cpu.cpp

 *
 *  Created on: Feb 18, 2016
 *      Author: angie
 */
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string>
#include "cpu.h"
#include"memory.h"
#include"message.h"
using namespace std;

    void cpu::setPipeCPU(int psd1,int psd2){     //set pipe for cpu
		p3=psd1;
		p4=psd2;
	}
    void cpu::setTimerCount(int c){
    	COUNT=c;
    }

	void cpu::cpuWrite(int addr,char c){
		m.arrIndex=addr;
		m.msg=c;
		write(p3, &m, sizeof(m));    //write the message to p3
		if(m.msg=='E'){              //close pipe
			close(p3);
		    close(p4);
		    exit(0);
		}
	}
	int cpu::cpuRead(){         //read message from p4
		read(p4, &m, sizeof(m));
		if(m.msg=='V'){          //memory violation,close pipe
			close(p3);
			close(p4);
			exit(0);
			return 0;
		}
		else{
		    return m.data;
		}
	}

    void cpu::cpuExecute(){    //execute instruction
    	char r='R';
    	char w='W';
    	char e='E';
    	int tmp;
    	int tmp1;
    	int pc;
    	SP=1000;             //SP register initialize
    	int count=0;         //counter initialize
    	system_mode = false;
    	while(PC<2000){
    		cpuWrite(PC,r);   // call function cpuWrite
    		IR = cpuRead();   //get the value from memory and assign it to register IR
    		PC++;
    		switch(IR){
    		case 1:           //Load the value into the AC
    			cpuWrite(PC,r);
    			tmp = cpuRead();
    			load(tmp);
    			PC++;
    			break;
    		case 2:         //Load the value at the address into the AC
    			pc=PC;
    			cpuWrite(PC,r);
    			PC=cpuRead();
    			cpuWrite(PC,r);
    			AC=cpuRead();
    			PC=pc;
    			PC+= 1;
    			break;
    		case 3:             //Load the value from the address found in the given address into the AC
    			cpuWrite(PC,r);
    			PC=cpuRead();
    			cpuWrite(PC,r);
    			PC=cpuRead();
    			break;
    		case 4:           //Load the value at (address+X) into the AC
    		    pc=PC;
    		    cpuWrite(PC,r);
    		    tmp=cpuRead();
    		    PC=X+tmp;
    		    cpuWrite(PC,r);
    		    tmp1=cpuRead();
    		    load(tmp1);
    		    PC=pc;
    		    PC++;
    		    break;
    		case 5:    //Load the value at (address+Y) into the AC
    		    pc=PC;
    		    cpuWrite(PC,r);
    		    tmp=cpuRead();
    		    PC=Y+tmp;
    		    cpuWrite(PC,r);
    		    tmp1=cpuRead();
    		    load(tmp1);
    		    PC=pc;
    		    PC++;
    		    break;
    		case 6:      //Load from (Sp+X) into the AC
    		    cpuWrite(SP+X,r);
    		    AC=cpuRead();
    		    break;
    		case 7:       //Store the value in the AC into the address
    		    cpuWrite(PC,r);
    		    tmp=cpuRead();
    		    m.data = AC;
    		    cpuWrite(tmp,w);
    		    cpuRead();
    		    PC++;
    		    break;
    		case 8:      //Gets a random int from 1 to 100 into the AC
    		    get_AC();
    		    break;
    		case 9:    //if port=1, writes AC as an int,If port=2, writes AC as a char
    		    cpuWrite(PC,r);
    		    tmp = cpuRead();
    		    put_port(tmp);
    		    PC++;
    		    break;
    		case 10:    //Add the value in X to the AC
    		    AC+=X;
    		    break;
    		case 11:     //Add the value in Y to the AC
    		    AC+=Y;
    		    break;
    		case 12:    //12.Subtract the value in X from the AC
    		    AC=AC-X;
    		    break;
    		case 13:    //13.Subtract the value in Y from the AC
    		    AC=AC-Y;
    		    break;
    		case 14:    //Copy the value in the AC to X
    			X=AC;
    			break;
    		case 15:    //Copy the value in X to the AC
    			AC=X;
    			break;
    		case 16:     //Copy the value in the AC to Y
    		    Y=AC;
    		    break;
			case 17:     //Copy the value in Y to the AC
			    AC=Y;
			    break;
			case 18:     //Copy the value in AC to the SP
			   SP=AC;
			   break;
			case 19:     //Copy the value in SP to the AC
			   AC=SP;
			   break;
    		case 20:      //Jump to the address
    		    cpuWrite(PC,r);
    		    tmp = cpuRead();
    		    PC=jmp_addr(tmp);
    		    break;
    		case 21:       //jump to the address only if the value in the AC is zero
    			cpuWrite(PC,r);
    			tmp = cpuRead();
    			PC=jmpif(tmp);
                break;
    		case 22:      //Jump to the address only if the value in the AC is not zero
    		    cpuWrite(PC,r);
    		    tmp = cpuRead();
    		    PC=jmpifnot(tmp);
    		    break;
    		case 23:        //Push return address onto stack, jump to the address
    		    cpuWrite(PC,r);
    		    tmp1=cpuRead(); // tmp1 is jump address
    		    tmp=PC+1;
    		    SP--;
    		    m.data=tmp;	// return address
    		    cpuWrite(SP,w);
    		    cpuRead();
    		    PC=tmp1;
    		    break;
    		case 24:       //Pop return address from the stack, jump to the address
    		    cpuWrite(SP,r);
				PC=cpuRead();
				SP++;
				break;
    		case 25:      //Increment the value in X
    			X++;
    			break;
    		case 26:      //Decrement the value in X
    			X--;
    			break;
    		case 27:      //Push AC onto stack
    			SP=SP-1;
    			m.data=AC;
    			cpuWrite(SP,w);
    			cpuRead(); // must pair discard result
    			break;
    		case 28:     //Pop from stack into AC
    			cpuWrite(SP,r);
    			AC=cpuRead();
    			SP=SP+1;
    			break;
    		case 29:    //Perform system call
    			Int(1);
    			break;
    		case 30:	// IRet
    			system_mode = false;
    			cpuWrite(SP,r);
    			tmp=cpuRead();
    			SP++;
    			cpuWrite(SP,r);
    			PC=cpuRead();
    			SP++;
    			SP=tmp;
    			m.user_mode = true;
    			break;
    		case 50:        //end
    			cpuWrite(PC,e);
    			close(p3);
    			close(p4);
                exit(0);
    			break;
    		default:
    			cout<<"Error! "<<IR<<" is an invalid instruction!"<<endl;
    			return;
    		}

    		 if(count==COUNT){    //every COUNT instructions,execute timer()
    			 count=0;
    			 if (system_mode==false) {    //timer will delay the execution if cpu is performing system call
    				 Int(0);
				}
			}
			else{
				count++;
			}
    	}
    }
    //Interrupt
    void cpu::Int(bool flag){
    	system_mode = true;
    	char w='W';
        int tmp=SP;      //store SP(user stack) into a local variable tmp
        SP=2000;
        SP--;
        m.user_mode = false;
        m.data=PC;
        cpuWrite(SP,w);  //save PC on the system stack
        cpuRead();
        SP--;
        m.data=tmp;	//       SP on user stack
        cpuWrite(SP,w);   //save SP on the system stack
        cpuRead();
        if(flag==0){
        	PC=1000;	// timer
        }
        else{
        	PC=1500;	// int instruction
        }
     }
    //1.Load the value into the AC
   	void cpu::load (int val)
   	{
           AC = val;
   	}
       //8.Gets a random int from 1 to 100 into the AC
   	void cpu::get_AC(){
   		AC=rand()%100+1;
   	}
       //9.If port=1, writes AC as an int to the screen;If port=2, writes AC as a char to the screen
   void cpu::put_port(int port){
       	if (port==1){
       		cout<<(int)AC;
       	}
       	else if (port==2){
       		cout<<(char)AC;
       	}
       }
   	//20.jump to the address
   	int cpu::jmp_addr(int address){
   		return address;
   	}
   	//21.Jump to the address only if the value in the AC is zero
   	int cpu::jmpif(int address){
   		if (AC==0){
   			return address;
   		}
   		else
   			return ++PC;
   	}
   	//22.Jump to the address only if the value in the AC is not zero
       int cpu::jmpifnot(int address){
       	if(AC!=0){
       		return address ;
       	}
       	else
       		return ++PC;
       }






