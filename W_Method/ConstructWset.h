#include "myBFS.h"
#include "PkTable.h"
#include "OrderedSelect.h"

class W
{
public:
	W(FSM &specs, string &fileName, int fsmNo);
private:
	bool pref(string a, string b);
	void compactWset();
	void constructTestSuite(vector<string> &w_set, FSM &f);
	void writeTestSuite(string &filename, int fsmNo, string alg);
	vector<string> U;
	vector<string> testSuite;
};
void W::writeTestSuite(string &filename, int fsmNo, string alg)
{
	ofstream myOfile;
	myOfile.open(filename, ios::app);
	int totalInput = 0;
	for(int i = 0 ; i < testSuite.size() ; i++)
	{
		totalInput += testSuite[i].size();
	}
	myOfile <<"FSM number" << fsmNo+1 << " algorithm: "<<alg<< " "<< testSuite.size() << " "<<totalInput<<" ";
	for(int i = 0; i < testSuite.size() ; i++)
		myOfile << testSuite[i] << " ";
	myOfile<< endl;
}
bool W::pref(string a, string b)
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
void W::compactWset()
{
	vector<string> tempW;
	for(int i = 0 ; i < testSuite.size() ; i++)
	{
		for(int j = i+1 ; j < testSuite.size() ; j++)
		{
			if(testSuite[i]==testSuite[j])
				testSuite[j]="";
			if(pref(testSuite[i],testSuite[j]))
				testSuite[i]="";
			if(pref(testSuite[j],testSuite[i]))
				testSuite[j]="";
		}
	}
	for(int i = 0 ; i < testSuite.size() ; i++)
	{
		if(testSuite[i].size()>0)
			tempW.push_back(testSuite[i]);
	}
	testSuite.clear();
	testSuite=tempW;
	tempW.clear();
}
W::W(FSM &specs, string &fileName, int fsmNo)
{
	myBFS obj(specs,0);
	this->U = obj.bfs(specs,0);
	OrderedSelect minCS(specs);
	vector<string> minCS_w = minCS.returnW();
	constructTestSuite(minCS_w, specs);
	compactWset();
	writeTestSuite(fileName,fsmNo,"min ");
	
	for(int i = 0 ; i < testSuite.size() ; i++)
	{
		testSuite[i].clear();
	}
	testSuite.clear();

	PkTable gill(specs);
	vector<string> gill_w = gill.returnW();
	constructTestSuite(gill_w, specs);
	compactWset();
	writeTestSuite(fileName,fsmNo,"gill");
}
void W::constructTestSuite(vector<string> &w_set, FSM &f)
{
	for(int i = 0; i<U.size() ; i++)
	{
		for(int j = 0 ; j < w_set.size(); j++)
		{
			string temp = U[i] + w_set[j];
			testSuite.push_back(temp);
		}
	}
	char buf[6];
	for(int i = 0; i<U.size() ; i++)
	{
		for(int j = 0 ; j < w_set.size(); j++)
		{
			for(int k = 0 ; k < f.getInputs() ; k++)
			{
				string temp = U[i] + itoa(k,buf,10) + w_set[j];
				testSuite.push_back(temp);
			}
		}
	}
}