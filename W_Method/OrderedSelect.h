//#include "ReadFSMs.h"


struct myPair
{
	pair<int, int> currentStates;
	pair<int, int> originStates;
	bool separated;
	bool separated2;
};
struct weightedSequence
{
	int separated;
	vector<int> sequence;
};
class OrderedSelect
{
public:
	OrderedSelect(FSM &f);
	void printWset(string fname, int i, float t);
	vector<string> returnW();
	~OrderedSelect();
private:
	vector<string> W;
	int length;
	vector<int> currentInputSequence;
	int states;
	int inputs;
	int outputs;
	int activeElements;
	int allElements;
	vector<myPair> pairSet;
	void Start(FSM &f);
	void decreaseElements();
	int getActiveElements();
	vector<int> getInputSequence();
	int getAllElements();
	void increment(vector<int> &t);
	vector<int> retrieveInputSequence();
	void rippleAdd(int index, vector<int> &myArray);
	vector<int> retreiveFirstInput();
	void applyInputSequence(myPair &p, vector<int> &in, FSM &f);
	void applyInputSequence2(myPair &p, vector<int> &in, FSM &f);
	void mySort(vector<weightedSequence> &list);
	void compactWset();
	bool pref(string a, string b);
};

vector<string> OrderedSelect::returnW()
{
	return W;
}

OrderedSelect::~OrderedSelect()
{
	length = 0;
	currentInputSequence.clear();
	states = 0;
	inputs = 0;
	outputs = 0;
	activeElements = 0;
	allElements = 0;
	pairSet.clear();
	for(int i =0 ; i < W.size(); i++)
	{
		W[i].clear();
	}
	W.clear();

}
vector<int> OrderedSelect::getInputSequence()
{
	return currentInputSequence;
}
OrderedSelect::OrderedSelect(FSM &f)
{
	this->inputs = f.getInputs();
	this->outputs = f.getOutputs();
	this->states = f.getStates();

	allElements = states*(states-1)/2;
	activeElements = allElements;

	for(int i =0 ; i<states; i++)
	{
		for(int j = i+1 ; j <states; j++)
		{
			myPair temp;
			temp.originStates.first=i;
			temp.originStates.second=j;
			temp.currentStates.first=i;
			temp.currentStates.second=j;
			temp.separated = false;
			temp.separated2 = false;
			pairSet.push_back(temp);
		}
	}
	length = 1;
	currentInputSequence.assign(length,0);
	Start(f);
	compactWset();
}
bool OrderedSelect::pref(string a, string b)
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
void OrderedSelect::compactWset()
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
void OrderedSelect::printWset(string fname, int i, float t)
{
	ofstream myOfile;
	myOfile.open(fname, ios::app);

	myOfile <<"FSM number" << i+1 << " ";
	for(int i = 0; i < W.size() ; i++)
		myOfile << W[i] << " ";
	myOfile << t <<" msec."<< endl;

}
void OrderedSelect::applyInputSequence(myPair &p, vector<int> &in, FSM &f)
{
	int c1 = p.currentStates.first;
	int c2 = p.currentStates.second;
	int o1 = 0 ; 
	int o2 = 0 ;
	for(int i = 0 ; i < in.size() ; i++)
	{
		o1 = f.returnOutputValue(c1,in[i]);
		o2 = f.returnOutputValue(c2,in[i]);
		c1 = f.returnNextStateValue(c1,in[i]);
		c2 = f.returnNextStateValue(c2,in[i]);
		
		if(o1!=o2)
		{
			p.separated=true;
		}
	}
}
void OrderedSelect::applyInputSequence2(myPair &p, vector<int> &in, FSM &f)
{
	int c1 = p.currentStates.first;
	int c2 = p.currentStates.second;
	int o1 = 0 ; 
	int o2 = 0 ;
	for(int i = 0 ; i < in.size() ; i++)
	{
		o1 = f.returnOutputValue(c1,in[i]);
		o2 = f.returnOutputValue(c2,in[i]);
		c1 = f.returnNextStateValue(c1,in[i]);
		c2 = f.returnNextStateValue(c2,in[i]);
		
		if(o1!=o2)
		{
			p.separated2=true;
		}
	}
}
void OrderedSelect::mySort(vector<weightedSequence> &list)
{
	for(int i = 0 ; i < list.size() ; i++)
	{
		for(int j = i+1 ; j < list.size() ; j++)
		{
			if(list[i].separated<list[j].separated)
			{
				weightedSequence temp = list[i];
				list[i]= list[j];
				list[j] = temp;
			}
		}

	}
}
void OrderedSelect::Start(FSM &f)
{
	vector<int> inputSequence = retreiveFirstInput();

	int numberOfSeparated = 0 ;
	int max= INT_MIN; 
	int sequenceLength = inputSequence.size();
	vector<weightedSequence> listOfSequences;

	while(activeElements>0)
	{		
		for(int i = 0 ; i<pairSet.size(); i++)
		{
			applyInputSequence(pairSet[i],inputSequence, f);
			if(pairSet[i].separated)
			{
				numberOfSeparated++;					
			}
		}
		for(int l = 0 ; l < pairSet.size(); l++)
		{
			pairSet[l].separated=false;
		}
		weightedSequence temp;
		temp.separated = numberOfSeparated;
		temp.sequence = getInputSequence();
		listOfSequences.push_back(temp);
		numberOfSeparated=0;

		inputSequence = retrieveInputSequence();
		
		if(sequenceLength<inputSequence.size())//length l finished
		{
			sequenceLength = inputSequence.size();
			mySort(listOfSequences);
			for(int l = 0 ; l < listOfSequences.size() && activeElements>0; l++)
			{
				for(int i = 0 ; i<pairSet.size()&& activeElements>0; i++)
				{
					if(!pairSet[i].separated2)
					{
						applyInputSequence2(pairSet[i],listOfSequences[l].sequence, f);
						if(pairSet[i].separated2)
						{
							decreaseElements();
							string t;
							char buf[6];
							for(int b= 0 ; b<listOfSequences[l].sequence.size(); b++)
							{
								t.append(itoa(listOfSequences[l].sequence[b],buf,10));
							}
							W.push_back(t);
						}
					}
				}
			}
			listOfSequences.clear();
		}
	}
}
vector<int> OrderedSelect::retreiveFirstInput()
{
	return this->currentInputSequence;
}
void OrderedSelect::increment(vector<int> &myArray)
{
	int firstIndex = 0 ;
	currentInputSequence[firstIndex]++;
	if(currentInputSequence[firstIndex] >= inputs)
	{
		rippleAdd(firstIndex,myArray);
	}
}
void OrderedSelect::rippleAdd(int index, vector<int> &myArray)
{
	if(myArray.size()>index)
	{
		myArray[index]=0;
	}
	if(myArray.size()>index+1)
	{
		int currentIndex = index+1;
		currentInputSequence[currentIndex]++;
		if(currentInputSequence[currentIndex] >= inputs)
		{
			rippleAdd(currentIndex,myArray);
		}
	}
	else{
		length++;
		currentInputSequence.assign(length,0);
	}
}
vector<int> OrderedSelect::retrieveInputSequence()
{
	int firstIndex = 0;
	if(currentInputSequence[firstIndex]+1 >= inputs)
	{
		increment(currentInputSequence);
	}
	else
	{
		currentInputSequence[firstIndex]++;
	}
	return currentInputSequence;
}
int OrderedSelect::getActiveElements()
{
	return this->activeElements;
}
int OrderedSelect::getAllElements()
{
	return this->allElements;
}
void OrderedSelect::decreaseElements()
{
	if(activeElements >0)
		activeElements--;
	else
	{
		cout<<"All pairs are distinguished"<<endl;
	}
}