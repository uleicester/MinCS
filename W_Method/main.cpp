#include <iostream>
#include <fstream>
#include "ConstructWset.h"


using namespace std;
int main()
{
	ReadFSMs reader("fsm.txt");
	string fileName = "out.txt";
	for(int i = 0 ; i< reader.FSMlist.size() ; i++)
	{
		W w(reader.FSMlist[i],fileName,i);		
	}
	return 0;
}