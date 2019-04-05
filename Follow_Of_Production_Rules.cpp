#include<bits/stdc++.h>

using namespace std;

struct follow_follow_terminals{
    set<char> follow;
    set<char> follow_terminals;
};

set<char> firstof(map<char, set<char> > First, string q, map<char, bool> isTerminal)
{

    set<char> first_of_q;
    for(int i=0;i<q.size();i++)
    {
        if(isTerminal[q[i]])
        {
            first_of_q.insert(q[i]);
            return first_of_q;
        }
        else{
            char B = q[i];
            first_of_q.insert(First[B].begin(), First[B].end());
            if(First[B].count('{')==0)
            {
                return first_of_q;
            }
        }
    }
    return first_of_q;
}

void find_follow(map<char, follow_follow_terminals> &Follow ,map<char, set<char> > First, vector<char> terminals, vector<char> non_terminals,
    map<char, vector<string> > production_rules, map<char, bool> isTerminal, map<char, bool> isNull, map<char, bool> &should_contain_null)
{
    map<char, vector<string> > ::iterator itr = production_rules.begin();
    for(;itr!=production_rules.end();itr++)
    {
        vector<string> rules = itr->second;
        for(int i=0;i<rules.size();i++)
        {
            string rule = rules[i];
            char S =itr->first;
            if(rule.size()==1)
            {
                if(!isTerminal[rule[0]])
                {
                    char R =rule[0];
                    Follow[R].follow.insert(S);
                }
            }
            else
            {
                for(int j=0;j<rule.size()-1;j++)
                {
                    char R = rule[j];
                    if(isTerminal[R])
                        continue;
                    string q = rule.substr(j+1, rule.size());
                    set<char>first_of_q = firstof(First, q, isTerminal);
                    Follow[R].follow_terminals.insert(first_of_q.begin(), first_of_q.end());

                    if(first_of_q.count('{') != 0 )
                    {
                     Follow[R].follow_terminals.erase('{');
                     Follow[R].follow.insert(S);
                    }
                }
                char R = rule[rule.size()-1];
                if(!isTerminal[R])
                    Follow[R].follow.insert(S);
            }
        }
    }
}

void find_first(char symbol, map<char, set<char> > &First, map<char, vector<string> > production_rules,
                             map<char, bool> isTerminal, map<char, bool> isNull, map<char, bool> &should_contain_null)
{
    vector<string> rules = production_rules[symbol];
    for(int i=0;i<rules.size();i++)
    {
        string rule = rules[i];

        if(isTerminal[rule[0]])
        {

            First[symbol].insert(rule[0]);
        }
        else if(rule[0]=='{')
        {

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


void create_follow_set(char symbol, map<char, follow_follow_terminals> Follow_struct, map<char, set<char> > &Follow)
{
        set<char> follow_actual_set;
        follow_actual_set.insert(Follow_struct[symbol].follow_terminals.begin(), Follow_struct[symbol].follow_terminals.end());
        if(!Follow_struct[symbol].follow.empty())
        {
            set<char> follow_set = Follow_struct[symbol].follow;
            set<char>::iterator itr=follow_set.begin();
            for(;itr!=follow_set.end();itr++)
            {
                 create_follow_set(*itr, Follow_struct, Follow);
                 follow_actual_set.insert(Follow[*itr].begin(), Follow[*itr].end());
            }
        }
        Follow[symbol].insert(follow_actual_set.begin(), follow_actual_set.end());
}

void find_Follow(char start_symbol, vector<char> terminals, vector<char> non_terminals, map<char,
                                        vector<string> > production_rules, map<char, bool> isTerminal, map<char, bool> isNull)
{

    map<char, set<char> > First;
    map<char, set<char> >Follow;
    map<char, follow_follow_terminals > Follow_struct;
    map<char, bool> should_contain_null;
    for(int i=0;i<non_terminals.size();i++)
    {
        char symbol=non_terminals[i];
        vector<string> rule;
        rule = production_rules[symbol];

        find_first(symbol, First, production_rules, isTerminal, isNull, should_contain_null);
    }

    for(int i=0;i<terminals.size();i++)
    {
        First[terminals[i]].insert(terminals[i]);
    }

    for(int i=0;i<non_terminals.size();i++)
    {
        char symbol = non_terminals[i];
        Follow_struct[symbol].follow.clear();
        Follow_struct[symbol].follow_terminals.clear();
    }

    Follow_struct[start_symbol].follow_terminals.insert('$');
    find_follow(Follow_struct, First, terminals, non_terminals, production_rules, isTerminal, isNull, should_contain_null);

    for(int i=0;i<non_terminals.size();i++)
    {
        char symbol = non_terminals[i];
        create_follow_set(symbol, Follow_struct, Follow);
    }


    map<char, set<char> >::iterator itr=Follow.begin();
    cout<<endl;
    while(itr!=Follow.end())
    {
        cout<<"Follow( ";
        cout<<itr->first<<" ) : "<<" [ ";
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
    bool ss=true;
    char start_symbol;
    isTerminal['{']= false;
    cout<<"Enter number of Non Terminals : ";
    cin>>num_non_terminals;
    cout<<"\nEnter non terminal symbols (start symbol first) : \n";
    for(int i=0;i<num_non_terminals;i++)
    {
        char ch;
        cin>>ch;
        if(ss)
        {
            start_symbol = ch;
            ss=!ss;
        }
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
    cout<<"Enter Production Rules(A->BC), use '{'  for null \n";
    for(int i=0;i<num_prod_rules;i++)
    {
        char start;
        string rule;
        cin>>rule;
        start = rule[0];
        rule = rule.substr(3,rule.size());
        production_rules[start].push_back(rule);
        if(rule[3]=='{')
            isNull[start] = true;
    }
    cout<<endl;

    find_Follow(start_symbol, terminals, non_terminals, production_rules, isTerminal, isNull);
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

A->BCD

B->b

B->{

C->c

C->{

D->d

D->{

Answer
A :  [ a, b, ]
B :  [ a, b, ]
C :  [ d, {, ]
D :  [ e, {, ]
E :  [ a, b, ]


*/
