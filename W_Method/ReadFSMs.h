#include <iostream>
#include <vector>
#include <set>
#include <iomanip>
#include <math.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include "FSM.h"

class ReadFSMs
{
public: 
	ReadFSMs(string fileName);
	vector<FSM> FSMlist;
private:
	void Start();
	void End();
	bool readAnFSM();
	ifstream inputFile;
};

ReadFSMs::ReadFSMs(string fileName)
{
	inputFile.open(fileName.c_str());
	if(!inputFile.is_open()){
		cout<<"File couldn't opened.Please check the file's name!!"<<endl;
		inputFile.close();
	}
	Start();
}
void ReadFSMs::Start()
{
	int counter = 0;
	while(counter<9999)
	{
		readAnFSM();
		counter++;
	}
	
	std::cout<<"counter FSMs processed "<<endl;
	End();
}
void ReadFSMs::End()
{
	cout<<"All FSMs are stored"<<endl;
	inputFile.close();
}
bool ReadFSMs::readAnFSM()
{
	string title;
	int number;
	inputFile >> title >> number;
	if(inputFile.eof())
        return false;
 
	
	int s, t;
	inputFile >> s >> t;

	FSM f(s,t,t/s,t/s);
	
	int counter = 0;
	int so,de,in,ou;
	char c;
	while(counter<t)
	{
		inputFile >> so >> de >> c >> ou;
		in = int(c)-97;
		so = so-1;
		de = de-1;
		f.setTransition(so,in,ou,de);
		counter++;
	}
	FSMlist.push_back(f);
	f.~FSM();
}