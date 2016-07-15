#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<math.h>
#include <string>
#include<stdio.h>
#include"memory.h"
#include"message.h"
using namespace std;

    void memory::setPipeMemory(int psd1,int psd2){   //set pipe for memory
		p1=psd1;
		p2=psd2;
	}
    void memory::memoryRead(){        //memory read from p1
    	read(p1, &m, sizeof(m));
    	}
	void memory::memoryWrite(char c){       // memory write in p2
		 write(p2, &m, sizeof(m));
		 while(m.msg=='V'){           //close pipe
			close(p1);
			close(p2);
			exit(0);
		}
	}
	void memory::memoryCheckmsg(){     //after read message from  p1,check the message

		if(m.msg=='R'){            // the message request reading the value from address m.arrIndex
			if(m.arrIndex>999&&(m.user_mode==true)){  //check if accessing system address in user mode
				cout<<"Memory violation:accessing system address "<<m.arrIndex<<" in user mode"<<endl;
				m.msg='V';            //'V' means violation
			}
			else{
				m.data=read1(m.arrIndex);   //read the value from address m.arrIndex
			}

		}
		else if(m.msg=='W'){    // the message request writing m.data into address m.arrIndex
		  if(m.arrIndex>999&&(m.user_mode==true)){  //check if accessing system address in user mode
			    cout<<"Memory violation:accessing system address "<<m.arrIndex<<" in user mode"<<endl;
			    m.msg='V';      //'V' means violation
			}
			else{
			write1(m.arrIndex,m.data);    //write m.data into address m.arrIndex
			}
		}
		else if(m.msg=='E'){
			close(p1);
			close(p2);
			exit(0);
		}

	}
	//       read(address) -  returns the value at the address
	int memory::read1(int address) {
		return arr[address];
	}
	//writes the data to the address
	void memory::write1(int address, int val) {
		arr[address] = val;
	}
	//reading a program file
	int memory::readFile(char* p) {
		ifstream file;
		string line;
		file.open(p);
		if (!file)
		{
			cout << "open file error!" << endl;
			return -1;
		}
		int i = 0;
		string str[2000];     //declare  a string array
		while (getline(file, line)){
			str[i] = line;//read each line and store into str
			i++;
		}
			int row = 0;
			int arr_row = 0;
			for(row=0;row<i;row++) {
				//a line is blank,skip
				if (str[row].size() == 1 && str[row][0] == '\r') {
					continue;
				}
				//a line start with'.'
				else if (str[row][0] == '.') {
					str[row].erase(0,1);    //delete the first character of this line
					arr_row = atoi(str[row].c_str());   //convert string to int and assign to arr_row
					//cout<< "change address to: " << arr_row << endl;
					continue;
				}
				//normal line with comment
				else if(isdigit(str[row][0])){    //the first character of the line is digit
					int index = 0;
				    while(isdigit(str[row][index])) {
					  index++;                 //index moves to  next to the last digit
					}
					str[row].erase(index, str[row].size()-index); //delete the characters after the last digit
					arr[arr_row] = atoi(str[row].c_str());
					//cout<< "(" << arr_row << ")" << arr[arr_row]<<endl;
					arr_row++;
				}
				//a line with spaces first,then start with slash and comments,skip
				else {
					continue;
				}
		}
        return 0;
	}


