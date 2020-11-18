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


using namespace std;

class FSM{

	public:
		FSM(int s, int t, int i, int o);
		void setTransition(int so, int in, int ou, int de);
		~FSM();
		int getInputs();
		int getOutputs();
		int getStates();
		int returnOutputValue(int s, int i);
		int returnNextStateValue(int s, int i);
	private:
		vector< vector < pair<int,int> >> transitions;
		int number_of_states;
		int number_of_transitions;
		int number_of_inputs;
		int number_of_outputs;
		
};
void FSM::setTransition(int so, int in, int ou, int de)
{
	transitions[so][in].first=ou;
	transitions[so][in].second=de;
}
FSM::FSM(int s, int t, int i, int o)
{
		number_of_states = s;
		number_of_transitions = t;
		number_of_inputs = i;
		number_of_outputs = o;
		pair<int,int> p;
		p.first=-1;
		p.second=-1;
		transitions.assign(s,vector<pair<int,int>>(i,p));
}
FSM::~FSM()
{
	for(int i = 0 ; i < transitions.size() ; i++)
		transitions[i].clear();
	transitions.clear();
	number_of_inputs = 0 ; 
	number_of_outputs = 0 ;
	number_of_states = 0 ;
	number_of_transitions = 0;
}
int FSM::getInputs()
{
	return this->number_of_inputs;
}

int FSM::getOutputs()
{
	return this->number_of_outputs;
}

int FSM::getStates()
{
	return this->number_of_states;
}

int FSM::returnOutputValue(int s, int i)
{
	return this->transitions[s][i].first;
}


int FSM::returnNextStateValue(int s, int i)
{
	return this->transitions[s][i].second;
}