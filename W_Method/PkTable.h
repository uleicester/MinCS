#include <iostream>
#include <numeric>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
//#include "ReadFSMs.h"

struct PairData{
	string separating_sequence;
	pair<int,int> currentStates;
	pair<int,int> initialStates;
	bool separated;
};

class PkTable
{
public:
	PkTable( FSM &f);
	vector<string> returnW();
	void printWset(string fname, int i, float t);
	~PkTable();
private:
	int inputs;
	int outputs;
	int states;
	vector<PairData> Pairs;
	vector<string> W;
	int counter;
	bool isSeparate(pair<int,int>p, int i, FSM &f);
	bool isMerge(pair<int,int>p, int i, FSM &f);
	void Start( FSM &f);
	void compactWset();
	bool pref(string a, string b);
	bool isLoop(pair<int,int>p, int i, FSM &f);
	void separate(PairData & Pairs, FSM &f);
	void itarate(PairData & Pairs, int i, FSM &f);
	int isNextPairSeparated(pair<int,int>p, PairData &P,int i, FSM &f);
	void separateRepeat(PairData &pairs, FSM &f);
};
vector<string> PkTable::returnW()
{
	return W;
}
PkTable::~PkTable()
{
	W.clear();
	for(int i = 0 ; i <Pairs.size() ; i++)
	{
		Pairs[i].separating_sequence.clear();
	}
	Pairs.clear();
	states=0;
	inputs=0;
	outputs=0;
}
PkTable::PkTable( FSM &f)
{
	counter = 0; 
	Start(f);
}
bool PkTable::pref(string a, string b)
{
	if(a.size()>b.size())
		return false;
	for(int i = 0 ; i < a.size() ; i++)
	{
		if(a[i]!=b[i])
			return false;
	}
	return true;
}
void PkTable::compactWset()
{
	vector<string> tempW;
	for(int i = 0 ; i < W.size() ; i++)
	{
		for(int j = i+1 ; j < W.size() ; j++)
		{
			if(W[i]==W[j])
				W[j]="";
			if(pref(W[i],W[j]))
				W[i]="";
			if(pref(W[j],W[i]))
				W[j]="";
		}
	}
	for(int i = 0 ; i < W.size() ; i++)
	{
		if(W[i].size()>0)
			tempW.push_back(W[i]);
	}
	W.clear();
	W=tempW;
	tempW.clear();
}
void PkTable::printWset(string fname, int i, float t)
{
	ofstream myOfile;
	myOfile.open(fname, ios::app);

	myOfile <<"FSM number" << i+1 << " ";
	for(int i = 0; i < W.size() ; i++)
		myOfile << W[i] << " ";
	myOfile << t <<" msec."<< endl;

}
void PkTable::Start( FSM &f)
{
	this->inputs = f.getInputs();
	this->outputs = f.getOutputs();
	this->states = f.getStates();
	for(int i =0 ; i<states; i++)
	{
		for(int j = i+1 ; j <states; j++)
		{
			pair<int,int>p;
			p.first=i;
			p.second=j;
			PairData temp;
			temp.initialStates=p;
			temp.currentStates=p;
			temp.separated=false;
			temp.separating_sequence="";
			Pairs.push_back(temp);
		}
	}

	for(int i=0; i<Pairs.size(); i++)
		separate(Pairs[i],f);

	int iterator = 0;
	int numbersOfPairs = (states*(states-1)/2);
	while(iterator< numbersOfPairs && counter < numbersOfPairs ){
		if(Pairs[iterator].separated==false)
		{
			separateRepeat(Pairs[iterator],f);
			if(Pairs[iterator].separating_sequence.size()>3)
				int rt=0;
		}
		iterator++;
		if(iterator==numbersOfPairs)
			iterator=0;
	}
	compactWset();

}
bool PkTable::isSeparate(pair<int,int>p, int i, FSM &f)
{
	int nextState1 = f.returnNextStateValue(p.first,i);
	int nextState2 = f.returnNextStateValue(p.second,i);
	int output1 = f.returnOutputValue(p.first,i);
	int output2 = f.returnOutputValue(p.second,i);
	if(output1!=output2)
		return true;
	return false;
}
bool PkTable::isMerge(pair<int,int>p, int i, FSM &f)
{
	int nextState1 = f.returnNextStateValue(p.first,i);
	int nextState2 = f.returnNextStateValue(p.second,i);
	int output1 = f.returnOutputValue(p.first,i);
	int output2 = f.returnOutputValue(p.second,i);
	if(nextState1==nextState2 && output1==output2)
		return true;
	return false;
}
bool PkTable::isLoop(pair<int,int>p, int i, FSM &f)
{
	int nextState1 = f.returnNextStateValue(p.first,i);
	int nextState2 = f.returnNextStateValue(p.second,i);
	int output1 = f.returnOutputValue(p.first,i);
	int output2 = f.returnOutputValue(p.second,i);
	if((output1==output2) && (nextState1==p.first || nextState1==p.second) && (nextState2==p.first || nextState2==p.second))
		return true;
	return false;
}
void PkTable::itarate(PairData &p, int i, FSM &f)
{
	char buf[6];
	p.currentStates.first = f.returnNextStateValue(p.currentStates.first,i);
	p.currentStates.second = f.returnNextStateValue(p.currentStates.second,i);
	p.separating_sequence += itoa(i,buf,10);
	
}

int PkTable::isNextPairSeparated(pair<int,int>p, PairData &P, int i, FSM &f)
{
	int nextState1 = f.returnNextStateValue(p.first,i);
	int nextState2 = f.returnNextStateValue(p.second,i);

	if(nextState1>nextState2)
	{
		int temp = nextState1;
		nextState1 = nextState2;
		nextState2 = temp;
	}
	int index=0;
	for(int k = 0 ; k < nextState1; k++)
	{
		index+=states-k-1;
	}
	index+=nextState2-nextState1-1;
	
	if(Pairs[index].separated){
		return index;
	}
	return -1;
}
void PkTable::separateRepeat(PairData &pairs, FSM &f)
{
	char buf[6];
	vector<pair<int,int>> splitter;
	pair<int,int> next;
	for(int i = 0 ; i < inputs && !pairs.separated ; i++)
	{
		if(isMerge(pairs.currentStates,i,f))
			;
		else if(isLoop(pairs.currentStates,i,f))
			;
		else
		{
			int result = isNextPairSeparated(pairs.currentStates,pairs,i,f);
			/*if(result >= 0 && Pairs[result].separating_sequence.length()<states-1)
			{	
				pairs.separating_sequence += itoa(i,buf,10) + Pairs[result].separating_sequence;
				pairs.separated=true;
				counter++;
				W.push_back(pairs.separating_sequence);
		
			}*/	
			if(result >= 0)
			{
				pair<int,int> temp;
				temp.first = i;//input
				temp.second = result;//index
				splitter.push_back(temp);
			}
		}
	}
	if(splitter.size()>0)
	{
		int min = INT_MAX;
		int splitterInput = -1;
		for(int i = 0 ; i < splitter.size(); i++)
		{
			if(Pairs[splitter[i].second].separating_sequence.length()<min)
			{
				min = Pairs[splitter[i].second].separating_sequence.length();
				splitterInput = i;
			}
		}
		pairs.separating_sequence += itoa(splitter[splitterInput].first,buf,10) + Pairs[splitter[splitterInput].second].separating_sequence;
		pairs.separated=true;
		counter++;
		W.push_back(pairs.separating_sequence);
	}
	
}
void PkTable::separate(PairData &pairs, FSM &f)
{
	char buf[6];
	vector<int> splitter;
	
	for(int i = 0 ; i < inputs && !pairs.separated ; i++)
	{
		if(isMerge(pairs.currentStates,i,f))
			;
		else if(isLoop(pairs.currentStates,i,f))
			;
		else if (isSeparate(pairs.currentStates,i,f))
		{
			pairs.separating_sequence += itoa(i,buf,10);
			W.push_back(pairs.separating_sequence);
			pairs.separated = true;
			counter++;
		}
	}
		//else
		//{
		//	int result = isNextPairSeparated(pairs.currentStates,pairs,i,f);
		//	if(result >= 0)
		//	{	
		//	/*	pairs.separating_sequence += itoa(i,buf,10) + Pairs[result].separating_sequence;
		//		pairs.separated=true;
		//		counter++;
		//		W.push_back(pairs.separating_sequence);*/
		//		splitter.push_back(i);
		//	}			
		//}
}

