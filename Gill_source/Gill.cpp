#include <iostream>
#include <fstream>
#include <chrono>
#include "PkTable.h"

using namespace std;
int main()
{
	ReadFSMs reader("fsm.txt");
	cout<<reader.FSMlist.size()<<" FSM has been read"<<endl;

	for(int i = 0 ; i< reader.FSMlist.size() ; i++)
	{
		auto start_time = std::chrono::high_resolution_clock::now();
		PkTable partition(reader.FSMlist[i]);
		auto end_time = std::chrono::high_resolution_clock::now();
		auto time = end_time - start_time;
		float t = time/std::chrono::milliseconds(1);
		partition.printWset("Out.txt", i, t);
		partition.~PkTable();
	}
	return 0;
}

