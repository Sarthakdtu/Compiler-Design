#include<iostream>
#include<string>
#include<vector>
using namespace std;

void findComments(vector<string> code){
    vector<int> comment;
    bool multi_line_comment=false;
    for(int i=0;i<code.size();i++)
    {
        string str = code[i];
        if(multi_line_comment)
        {
            comment.push_back(2);
            for(int j=str.size()-1;j>0;j--)
              {
                if(str[j-1]=='*' && str[j]=='/')
                multi_line_comment=false;
              }
        }
        else
        {
             if(str.size()>1)
             {
                if(str[0]=='/' && str[1]=='/')
                {
                    comment.push_back(1);
                }
                else if(str[0]=='/' && str[1]=='*')
                {
                    multi_line_comment=true;
                    comment.push_back(2);
                    for(int j=str.size()-1;j>0;j--)
                    {
                        if(str[j-1]=='*' && str[j]=='/')
                        multi_line_comment=false;
                    }
                }
                else
                {
                    comment.push_back(0);
                }
             }
             else{
                    comment.push_back(0);
             }
        }
      }

    for(int i=0;i<comment.size();i++)
    {
        if(comment[i]==1)
        {
            cout<<"Line "<<i+1<<" is a single line comment."<<endl;
        }
        else if(comment[i]==2)
        {
            cout<<"Multiline comment from line "<<i+1<<" to ";
            while(i<comment.size() && comment[i]==2)
                    i++;
            i--;
            cout<<"line "<<i+1<<".\n";
        }
        else
        {
            cout<<"Line "<<i+1<<" is a normal statement."<<endl;
        }
    }
}

int main()
{
    string str ="";
    vector<string> code;
    cout<<"Enter your code (enter \"end\" at the last line to mark end of the code) ";
    int i=1;
    cout<<endl<<endl;
    while(str!="end"){
        cout<<i<<" ";
        getline(cin, str);
        code.push_back(str);
        i++;
    }
    cout<<endl;
    findComments(code);
    return 0;
}
