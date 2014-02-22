//         /\_/\
//   _____/ o o \
//  /~____  =ø= /
// (______)__m_m)

// Aditya Makkar
// 2012ME10874
// Group 3

#include <iostream>
#include <cstring>

using namespace std;

int charToInt(char c)
{
    return (c-48); //'0'==48
}

char intToChar(int n) // assuming n is single digit
{
    return char(n+48);
}

int stringToInt(string s)
{
    size_t n = s.size();
    int ans = 0;
    for(int i = 0; i<n; ++i)
    {
        ans = 10*ans + charToInt(s[i]);
    }
    return ans;
}

string intToString(int n)
{
    if(n==0) return "0";
    string ans = "";
    while(n>0)
    {
        ans = intToChar(n%10) + ans;
        n/=10;
    }
    return ans;
}

string Substring(string s, int start, int length)
{
    string ans = "";
    for(int i=0; i<length; ++i)
    {
        ans+=s[start+i];
    }
    return ans;
}

struct node
{
    string data;
    int state;
    node* next;
};

class Stack
{
private:
    node* top;
    int size;
public:
    Stack();
    void Push(string s, int state);
    string TopData();
    int TopState();
    void Pop();
    bool isEmpty();
    int Size();
};

Stack::Stack()
{
    top = NULL;
    size = 0;
}

bool Stack::isEmpty()
{
    return (size==0);
}

int Stack::Size()
{
    return size;
}

void Stack::Push(string s, int isstate)
{
    node* Naya = new node;
    Naya->data = s;
    Naya->state = isstate;
    Naya->next = top;
    top = Naya;
    ++size;
}

string Stack::TopData()
{
    return top->data;
}

int Stack::TopState()
{
    return top->state;
}

void Stack::Pop()
{
    if(isEmpty()) return;
    node* newTop = top->next;
    delete top;
    top = newTop;
    --size;
}

bool judge(string * inputSequence, Stack & stac, Stack & rulesApplied, int numberOfRules, string ** rules, string * dummyRules2, int numberOfFinalSymbols, int numberOfNonFinalSymbols, string * symbols, int rowsOfLookupTable, int columnsOfLookupTable, string ** lookupTable)
{
    int lengthOf_inputSequence = stringToInt(inputSequence[0]);
    //cout<<"Entered judge"<<endl;
    //cout<<"lengthOf_inputSequence is -"<<lengthOf_inputSequence<<"-"<<endl;
    int I = 1;
    bool verdict = false;
    while(I<lengthOf_inputSequence+1)
    {
        //cout<<"Entered while"<<endl;
        int currentState = stac.TopState();
        //cout<<"current state is -"<<currentState<<"-"<<endl;
        int J = 0;
        bool semiVerdict = false;
        for(; J<numberOfFinalSymbols+numberOfNonFinalSymbols; ++J)
        {
            if(symbols[J]==inputSequence[I])
            {
                semiVerdict = true;
                //cout<<"J which breaks it is "<<J<<endl;
                break;
            }
        }
        if(!semiVerdict) return false;
        string whatToDo = lookupTable[currentState][J];
        //cout<<"whatToDo is -"<<whatToDo<<"-"<<endl;
        if(whatToDo=="-") return false;
        if(whatToDo=="acc") return true;
        if(whatToDo[0]=='p')
        {
            //cout<<"Entered p"<<endl;
            int whatToDo_number = stringToInt(Substring(whatToDo, 1, whatToDo.size()-1));
            //cout<<"whatToDo_number is -"<<whatToDo_number<<"-"<<endl;
            stac.Push(inputSequence[I], whatToDo_number);
            ++I;
        }
        else
        {
            //cout<<"else"<<endl;
            int whatToDo_number = stringToInt(Substring(whatToDo, 1, whatToDo.size()-1));
            //cout<<"whatToDo_number is -"<<whatToDo_number<<"-"<<endl;
            int lengthOfTail = stringToInt((rules[whatToDo_number-1][0])) - 2;
            //cout<<"lenghtOftail is "<<lengthOfTail<<endl;
            for(int i = lengthOfTail+2; i>2; --i)
            {
                if(rules[whatToDo_number-1][i]==stac.TopData())
                {
                    stac.Pop();
                    //cout<<"popped for "<<rules[whatToDo_number-1][i]<<endl;
                }
                else
                {
                    return false;
                }
            }
            rulesApplied.Push(dummyRules2[whatToDo_number-1], 0);
            //cout<<"pushed "<<dummyRules2[whatToDo_number-1]<<" in rulesApplied"<<endl;
            int j = 0;
            for(; j<numberOfNonFinalSymbols+numberOfFinalSymbols; ++j)
            {
                if(symbols[j]==rules[whatToDo_number-1][1])
                {
                    if(lookupTable[stac.TopState()][j]=="-") return false;
                    stac.Push(symbols[j], stringToInt(lookupTable[stac.TopState()][j]));
                    //cout<<"j which causes push is "<<j<<endl;
                }
            }
        }
    }
    return verdict;
}

void printReverse(Stack & rulesApplied)
{
    while(!rulesApplied.isEmpty())
    {
        cout<<rulesApplied.TopData();
        rulesApplied.Pop();
        cout<<endl;
    }
}

int main()
{
    //Taking Preliminary Input
    int numberOfRules; cin>>numberOfRules;
    //cout<<"numberOfRules is -"<<numberOfRules<<"-"<<endl;
    string dummy;
    getline(cin,dummy);
    //cout<<"dummy is -"<<dummy<<"-"<<endl;
    
    string ** rules = new string*[numberOfRules]; // rules is a pointer to pointer-to-string and it points to first element of a pointer-to-string array
    
    //taking input and storing it as a continuous string in dummyRules
    string *dummyRules = new string[numberOfRules];
    string *dummyRules2 = new string[numberOfRules];
    for(int i=0; i<numberOfRules; ++i)
    {
        //cout<<"Entered for loop"<<endl;
        getline(cin,dummy);
        //cout<<"dummy is -"<<dummy<<"-"<<endl;
        dummyRules2[i] = dummy;
        string rule_i = "";
        for(int j=0; j<dummy.size(); ++j)
        {
            if(dummy[j]==' ') continue;
            rule_i += dummy[j];
        }
        dummyRules[i] = rule_i;
        //cout<<"dummyRules[i] is -"<<dummyRules[i]<<"-"<<endl;
    }
    
    //taking symbols (final and non-final)
    int numberOfFinalSymbols, numberOfNonFinalSymbols;
    cin>>numberOfFinalSymbols;
    cin>>numberOfNonFinalSymbols;
    //cout<<"number of final & non-final symbols are -"<<numberOfFinalSymbols<<"-"<<numberOfNonFinalSymbols<<"-"<<endl;
    string *symbols = new string[numberOfNonFinalSymbols + numberOfFinalSymbols];
    for(int i=0; i<numberOfFinalSymbols+numberOfNonFinalSymbols; ++i)
    {
        cin>>symbols[i];
        //cout<<"symbols[i] is -"<<symbols[i]<<"-"<<endl;
    }
    
    //converting dummyRules to rules using symbols
    for(int i=0; i<numberOfRules; ++i)
    {
        int lengthOfRule_i = dummyRules[i].size();
        rules[i] = new string[lengthOfRule_i+1];
        int j = 0;
        int k = 1;
        while(j<lengthOfRule_i)
        {
            //cout<<"Entered outer while with j = "<<j<<endl;
            if(dummyRules[i][j]=='!')
            {
                rules[i][k]="!";
                ++j;
                ++k;
                //cout<<"! encountered"<<endl;
            }
            else
            {
                //cout<<"else entered"<<endl;
                string candidate;
                int len = 1;
                while(true)
                {
                    candidate = Substring(dummyRules[i], j, len);
                    //cout<<"a candidate is "<<candidate<<endl;
                    bool isThere = false;
                    for(int I = 0; I<numberOfFinalSymbols+numberOfNonFinalSymbols; ++I)
                    {
                        if(candidate==symbols[I])
                        {
                            isThere = true;
                            break;
                        }
                    }
                    if(isThere)
                    {
                        break;
                    }
                    ++len;
                }
                //cout<<"final candidate is "<<candidate<<endl;
                rules[i][k]=candidate;
                ++k;
                j+=len;
            }
        }
        rules[i][0] = intToString(k-1);
    }
    /*cout<<"---displaying rules---"<<endl;
    for(int i=0; i<numberOfRules; ++i)
    {
        for(int j=0; j<=stringToInt(rules[i][0]); ++j)
            cout<<rules[i][j]<<" ";
        cout<<endl;
    }*/
    
    // reading lookup table
    int rowsOfLookupTable, columnsOfLookupTable;
    cin>>rowsOfLookupTable>>columnsOfLookupTable;
    //cout<<"rows and columns of lookup table are -"<<rowsOfLookupTable<<"-"<<columnsOfLookupTable<<"-"<<endl;
    string ** lookupTable = new string*[rowsOfLookupTable];
    for(int i=0; i<rowsOfLookupTable; ++i)
    {
        lookupTable[i] = new string[columnsOfLookupTable];
        for(int j=0; j<columnsOfLookupTable; ++j)
        {
            cin>>lookupTable[i][j];
            //cout<<lookupTable[i][j]<<" ";
        }
        //cout<<endl;
    }
    
    //taking input sequences
    int numberOfInputSequences;
    cin>>numberOfInputSequences;
    //cout<<"numberOfInputSequences is -"<<numberOfInputSequences<<"-"<<endl;
    getline(cin,dummy);
    //cout<<"dummy is -"<<dummy<<"-"<<endl;
    for(int i=0; i<numberOfInputSequences; ++i)
    {
        //cout<<"entered for loop"<<endl;
        //taking as dummy, converting it to continuous anotherDummy and them separating it to indivisual symbols
        getline(cin,dummy);
        //cout<<"dummy is -"<<dummy<<"-"<<endl;
        string anotherDummy = "";
        for(int j=0; j<dummy.size(); ++j)
        {
            if(dummy[j]==' ') continue;
            anotherDummy += dummy[j];
        }
        int lengthOfDummy = anotherDummy.size();
        string * inputSequence = new string[lengthOfDummy+1];
        int j = 0;
        int k = 1;
        while(j<lengthOfDummy)
        {
            string candidate;
            int len = 1;
            while(true)
            {
                candidate = Substring(anotherDummy, j, len);
                bool isThere = false;
                for(int I = 0; I<numberOfFinalSymbols+numberOfNonFinalSymbols; ++I)
                {
                    if(candidate==symbols[I])
                    {
                        isThere = true;
                        break;
                    }
                }
                if(isThere)
                {
                    break;
                }
                ++len;
            }
            inputSequence[k] = candidate;
            ++k;
            j+=len;
        }
        inputSequence[0] = intToString(k-1);
        /*for(int ij=0; ij<k; ++ij)
            cout<<inputSequence[ij]<<" ";
        cout<<endl;*/
        
        //processing input sequence
        Stack stac;
        stac.Push("",0);
        Stack rulesApplied;
        if(judge(inputSequence, stac, rulesApplied, numberOfRules, rules, dummyRules2, numberOfFinalSymbols, numberOfNonFinalSymbols, symbols, rowsOfLookupTable, columnsOfLookupTable, lookupTable))
        {
            cout<<"accepted"<<endl;
            printReverse(rulesApplied);
        }
        else
        {
            cout<<"rejected"<<endl;
        }
        
        delete [] inputSequence;
    }

    //deleting
    for(int i=0; i<rowsOfLookupTable; ++i)
    {
        delete [] lookupTable[i];
    }
    delete [] lookupTable;
    delete [] dummyRules;
    delete [] symbols;
    for(int i=0; i<numberOfRules; ++i)
    {
        delete [] rules[i];
    }
    delete [] rules;
}