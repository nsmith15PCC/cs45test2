#include <iostream>
#include <bitset>
#include <string>

using namespace std;

void include(unsigned short int &universe, size_t who);
void display(unsigned short int universe);
size_t find(unsigned short int **allsets, char tofind);
void initialize(unsigned short int **allsets);
void help();
void perform(unsigned short int **allsets, string input);
void list(unsigned short int **allsets, string content);
void set(unsigned short int **allsets, string content);
unsigned short int setUnion(unsigned short int first, unsigned short int second);
unsigned short int setIntersection(unsigned short int first, unsigned short int second);
unsigned short int setDifference(unsigned short int first, unsigned short int second);

int main()
{
    unsigned short int allsets[26][2];


//    help();




        unsigned short int universe = 0;
        size_t bit;
        while(1)
        {
            cout<<"Who to include? ";
            cin>>bit;
            include(universe,bit);
            display(universe);
            display(~universe);
        }
    return 0;
}


void include(unsigned short int &universe, size_t who)
{
    unsigned short int mask = 1;
    universe |= mask << who;
    cout<<"Universe is: "<<universe<<endl;
}

void display(unsigned short int universe)
{
    unsigned short int mask = 1;
    cout<<"Members of the set are: ";
    for(unsigned short int i = 0; i < 16; ++i)
    {
        unsigned short int result;
        result = (mask << i) & universe;
        if(result)
            cout<<i<<", ";
    }
    cout<<"\b\b "<<endl;
}

size_t find(unsigned short int **allsets, char tofind)
{
    for (size_t i = 0; i<26; ++i)
        if (allsets[i][0] != 0 && allsets[i][0] == int(tofind))
            return i;
    return 26;
}

void initialize(unsigned short int **allsets)
{
    for (size_t i = 0; i < 26; ++i)
        for (size_t j = 0; j < 2 ; ++j)
            allsets[i][j] = 0;
    return;
}

void help()
{
    cout<<"Commands:"<<endl
       <<"LIST - List all sets and their contents"<<endl
      <<"HELP - Give help"<<endl
     <<"SET - Enter a set (can be a definition, union, intersection, or combination of these)"<<endl
    <<"'+' - Returns the union of two sets"<<endl
    <<"'*' - Returns the intersection of two sets"<<endl
    <<"LOAD <filename> - Load a list of sets from file"<<endl
    <<"SAVE <filename> - Save a list of sets to file"<<endl;
    return;
}

void perform(unsigned short int **allsets, string input)
{
    size_t pos = input.find(' ');
    string content;
    if (pos != string::npos)
    {
        content = input.substr(pos+1);
        input = input.substr(0,pos);
    }
    for (size_t i = 0; i<input.size();++i)
        input[i] = toupper(input[i]);
    if (input == "LIST" && pos != string::npos)
        list(allsets, content);
    else if (input == "SET" && pos != string::npos)
        set(allsets, content);
    else
        help();

}

void list(unsigned short int **allsets, string content)
{
    size_t i(0);
    while (allsets[i][0] != 0)
    {
        cout<<char(allsets[i][0])<<" = ";
        display(allsets[i][1]);
                cout<<endl;
        ++i;
    }
}

void set(unsigned short int **allsets, string content){}

unsigned short int setUnion(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    for (size_t i = 0; i < 16; ++i)
    {
        (third << i) = (first << i) | (second << i);
    }
    return third;
}

unsigned short int setIntersection(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    for (size_t i = 0; i < 16; ++i)
    {
        (third << i) = (first << i) & (second << i);
    }
    return third;
}

unsigned short int setDifference(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    for (size_t i = 0; i < 16; ++i)
    {
        (third << i) = (first << i) & !(second << i);
    }
    return third;
}
