#include<bits/stdc++.h>

using namespace std;

void find_first(char symbol, map<char, set<char> > &First, map<char, vector<string> > production_rules,
                             map<char, bool> isTerminal, map<char, bool> isNull, map<char, bool> &should_contain_null)
{
    vector<string> rules = production_rules[symbol];
    for(int i=0;i<rules.size();i++)
    {
        string rule = rules[i];

        if(isTerminal[rule[0]])
        {
            //cout<<"Terminal ";
            First[symbol].insert(rule[0]);
        }
        else if(rule[0]=='{')
        {
            //cout<<"NULL";
            First[symbol].insert(rule[0]);
            should_contain_null[symbol]=true;
        }
        else{
            for(int i=0;i<rule.size();i++)
            {
                char symbol_next = rule[i];

                if(isTerminal[rule[i]]){
                    First[symbol].insert(rule[i]);
                }
                else{
                    find_first(symbol_next, First, production_rules, isTerminal, isNull, should_contain_null);
                    First[symbol].insert(First[symbol_next].begin(), First[symbol_next].end());
                }

                if(!isNull[rule[i]])
                    break;
                if(i == rule.size()-1)
                {
                    should_contain_null[symbol] = true;
                }
            }
        }
    }
}

void find_First(vector<char> terminals, vector<char> non_terminals, map<char,
                                        vector<string> > production_rules, map<char, bool> isTerminal, map<char, bool> isNull)
{

    map<char, set<char> > First;
    map<char, bool> should_contain_null;
    for(int i=0;i<non_terminals.size();i++)
    {
        char symbol=non_terminals[i];
        vector<string> rule;
        rule = production_rules[symbol];

        find_first(symbol, First, production_rules, isTerminal, isNull, should_contain_null);

    }


    map<char, set<char> >::iterator itr=First.begin();
    while(itr!=First.end())
    {
        cout<<itr->first<<" : "<<" [ ";
        set<char> f = itr->second;
        if(f.count('{')>0 && !should_contain_null[itr->first])
            f.erase('{');
        for(set<char> ::iterator it=f.begin();it!=f.end();it++)
            cout<<*it<<", ";
        cout<<"]\n";
        itr++;
    }

}

void input(){
    int num_non_terminals, num_terminals, num_prod_rules;

    vector<char> terminals,non_terminals;
    map<char, bool> isTerminal, isNull;
    map<char, vector<string> > production_rules;

    isTerminal['{']= false;
    cout<<"Enter number of Non Terminals : ";
    cin>>num_non_terminals;
    cout<<"\nEnter non terminal symbols : \n";
    for(int i=0;i<num_non_terminals;i++)
    {
        char ch;
        cin>>ch;
        non_terminals.push_back(ch);
        isTerminal[ch]=false;
        isNull[ch] = false;
    }
    cout<<endl;
    cout<<"Enter number of Terminals : ";
    cin>>num_terminals;
    cout<<"\nEnter terminal symbols : \n";
    for(int i=0;i<num_terminals;i++)
    {
        char ch;
        cin>>ch;
        terminals.push_back(ch);
        isTerminal[ch]=true;
        isNull[ch]=false;
    }
    cout<<endl;
    cout<<"Enter number of Production Rules : ";
    cin>>num_prod_rules;
    cout<<endl;
    cout<<"Enter Production Rules, use '{'  for null \n";
    for(int i=0;i<num_prod_rules;i++)
    {
        char start;
        string rule;
        cin>>rule;
        start = rule[0];
        rule = rule.substr(3,rule.size());
        //cout<<"Rule : "<<rule;
        production_rules[start].push_back(rule);
        if(rule[3]=='{')
            isNull[start] = true;
        cout<<endl;
    }
    cout<<endl;

    find_First(terminals, non_terminals, production_rules, isTerminal, isNull);
}

int main(){
    input();
}

/*
5

A
B
C
D
E

5

1
2
3
4
5

8

A->BC

C->dBC

C->{

B->ED

E->bAc

E->a

D->eED

D->{

Answer
A :  [ a, b, ]
B :  [ a, b, ]
C :  [ d, {, ]
D :  [ e, {, ]
E :  [ a, b, ]


*/
