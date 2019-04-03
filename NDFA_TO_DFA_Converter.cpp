#include<iostream>
#include<map>
#include<algorithm>
#include<sstream>
#include<queue>
#include<vector>
#include<set>
#include<string>
using namespace std;

void displayStates(set<string> states)
{
	cout<<"\n------------------------States-------------------------------\n";
	set<string> ::iterator it = states.begin();
	//int stateNum = 0;
	while(it!=states.end())
	{
		string state = *it;
		cout<<state<<endl;
		it++;
	}
	cout<<"\n-------------------------------------------------------------\n";
}

void displayDfa(map<string, string>zero, map<string, string> one, set<string> states)
{
	cout<<"States  \t0\t|\t\t1\n";
	states.insert("0");
	set<string> ::iterator it = states.begin();
	int stateNum =0;
	//cout<<"q0"<<"\t"<<zero["0"]<<"\t"<<one["0"];
	while(it!=states.end())
	{
		string state = *it;
		cout<<"q"<<state<<"\t";
		if(zero[state]!="")
		cout<<"q"<<zero[state]<<"\t";
		if(one[state]!="")
		cout<<"q"<<one[state]<<"\t";
		cout<<endl;
		stateNum++;
		it++;
	}
}

void displayNdfa(vector<vector<int>> zero, vector<vector<int>> one)
{
	cout<<"States  \t0\t|\t\t1\n";
	for(int i=0;i<zero.size();i++)
	{
		cout<<"Q"<<i<<"  \t  ";
	    for(int j=0;j<zero[i].size();j++)
	       cout<<"Q"<<zero[i][j]<<",";
		cout<<"\t\t\t";
		for(int j=0;j<one[i].size();j++)
	       cout<<"Q"<<one[i][j]<<",";
		cout<<endl;
	}
}

set<int> removeDuplicate(string ip, int n)
{
	set<int> unique;
	for(int j=0;j<ip.size();j++)
        {
            if(ip[j]==' '||ip[j]==','||isalpha(ip[j])==1)
                continue;
            else
            {
            	if(ip[j]-'0'>n-1||ip[j]-'0'<0)
            	{
            		continue;
				}
            	unique.insert(ip[j]-'0');
			}
        }

	return unique;
}

string make_string(set<int> unique, string ip)
{
	set<int> ::iterator it=unique.begin();
	ip="";
        while(it!=unique.end())
        {
        	ip += (*it)+48;
        	it++;
		}
		return ip;
}

void convert(vector<vector<int>> zero, vector<vector<int>> one)
{
	int n = zero.size();
	map<string, string> dfa0, dfa1;
	queue<string> states;
	set<string> stateSet;
	string ip="";

	for(int i=0;i<zero[0].size();i++)
		ip += zero[0][i]+48;
	set<int> unique = removeDuplicate(ip, n);

	ip=make_string(unique, ip);
	dfa0["0"] = ip;
	//cout<<make_string(unique, ip)<<" is the new state"<<endl;
	states.push(ip);
	stateSet.insert(ip);
	ip="";
	for(int i=0;i<one[0].size();i++)
		ip += one[0][i]+48;
	unique = removeDuplicate(ip, n);
	//ip="";
	ip = make_string(unique, ip);
	dfa1["0"] = ip;
	//cout<<make_string(unique, ip)<<" is the new state"<<endl;
	//dfa1["0"] = ip;
	if(stateSet.count(ip)==0)
	states.push(ip);
	stateSet.insert(ip);
	//displayStates(states);
	//set<string> ::iterator it=states.begin();

	while(!states.empty())
	{
		string state = states.front();
		states.pop();
		//cout<<"For State "<<state<<endl;
		ip="";
		for(int i=0;i<state.size();i++)
		{
			int idx = state[i]-48;
			for(int j=0;j<zero[idx].size();j++)
			{
				ip += zero[idx][j]+48;
				//cout<<"Curr ip : "<<ip<<endl;
			}
		}
		//cout<<"\nOriginal String is "<<ip<<endl;
		unique = removeDuplicate(ip, n);
		//ip="";
		ip = make_string(unique, ip);
	    dfa0[state] = ip;
	    if(stateSet.count(ip)==0)
	    states.push(ip);
		stateSet.insert(ip);

		//cout<<make_string(unique, ip)<<" is the new state"<<endl;

		ip="";
		for(int i=0;i<state.size();i++)
		{
			int idx = state[i]-48;
			for(int j=0;j<one[idx].size();j++)
			{
				ip += one[idx][j]+48;
				//cout<<"Curr ip : "<<ip<<endl;
			}
		}
		//cout<<"\nOriginal String is "<<ip<<endl;
		unique = removeDuplicate(ip, n);
		//ip="";
		ip = make_string(unique, ip);
	    dfa1[state] = ip;
	    if(stateSet.count(ip)==0)
		states.push(ip);
		stateSet.insert(ip);
	    //states.insert(ip);
	    //cout<<make_string(unique, ip)<<" is the new state"<<endl;
		//cout<<dfa1[state]<<endl;
		//it++;
		//displayStates(stateSet);
		//break;
	}

	displayDfa(dfa0, dfa1, stateSet);
}

void enterTransitionTable(int n)
{
	vector<vector<int>> zero = vector<vector<int>>(n);
	vector<vector<int>> one = vector<vector<int>>(n);

	for(int i=0;i<n;i++)
    {
    	string ip;
    	cout<<"-------------------------------------------\n";
    	cout<<"          State Q"<<i<<endl;
    	//ip="aa";
        cout<<"Enter state transitions for input 0 : ";
        if(i==0){getline(cin, ip);}
		getline(cin, ip);

        set<int> unique = removeDuplicate(ip, n);
        set<int> ::iterator it=unique.begin();
        while(it!=unique.end())
        {
        	int elt = int(*it);
        	zero[i].push_back(*it);
        	it++;
		}
        ip="";
        cout<<"Enter state transitions for input 1 : ";
        //string ip1;
        getline(cin, ip);
        unique = removeDuplicate(ip, n);
        it=unique.begin();
        while(it!=unique.end())
        {
        	one[i].push_back(*it);
        	it++;
		}
        cout<<"\n-------------------------------------------\n";
    }
    displayNdfa(zero, one);
    convert(zero, one);
}


int main()
{
	int n;
	cout<<"Enter number of states : ";
	cin>>n;
	cout<<endl;
	enterTransitionTable(n);
	return 0;
}
