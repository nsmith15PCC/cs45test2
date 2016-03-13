#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void include(unsigned short int &universe, size_t who);
void display(unsigned short int universe);
size_t find(const unsigned short int allsets[][2], char tofind);
void initialize(unsigned short int allsets[][2]);
void help();
void perform(unsigned short int allsets[][2], string input);
void list(unsigned short int allsets[][2], string content);
void set(unsigned short int allsets[][2], string content);
unsigned short int setUnion(unsigned short int first, unsigned short int second);
unsigned short int setIntersection(unsigned short int first, unsigned short int second);
unsigned short int setDifference(unsigned short int first, unsigned short int second);
void evaluate_vects (vector<unsigned short int> &operands, vector<char> &operators);
unsigned short int evaluate (const unsigned short int allsets[][2], string content);
void replaceLetters(const unsigned short int allsets[][2], string &expression);
unsigned short int setFromList(string content);
void replaceBrackets(const unsigned short int allsets[][2], string &expression);
void save(const unsigned short int allsets[][2], string filename);
void load(unsigned short int allsets[][2], string filename);

int main()
{
    unsigned short int allsets[26][2];
    initialize(allsets);
    string input;

    help();
    getline(cin, input);
    while(input!= "")
    {
        perform(allsets,input);
        //        help();
        getline(cin, input);

    }

    return 0;
}


void include(unsigned short int &universe, size_t who)
{
    unsigned short int mask = 1;
    universe |= mask << who;
    //    cout<<"Universe is: "<<universe<<endl;
}

void display(unsigned short int universe)
{
    unsigned short int mask = 1;
    bool notempty;
    //    cout<<"Members of the set are: ";
    for(unsigned short int i = 0; i < 16; ++i)
    {
        unsigned short int result;
        result = (mask << i) & universe;
        if(result)
        {
            notempty = true;
            cout<<i<<", ";
        }
    }
    if (notempty)
    cout<<"\b\b ";
    else
        cout<<"NULL";
}

size_t find(const unsigned short int allsets[][2], char tofind)
{
    size_t i = 0;
    while (i < 26 && allsets[i][0] != 0 && allsets[i][0] != int(tofind))
        ++i;
    return i;
}

void initialize(unsigned short int allsets[][2])
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
    <<"    '+' - Returns the union of two sets"<<endl
    <<"    '*' - Returns the intersection of two sets"<<endl
    <<"    '*' - Returns the difference of two sets"<<endl
    <<"LOAD <filename> - Load a list of sets from file"<<endl
    <<"SAVE <filename> - Save a list of sets to file"<<endl;
    return;
}

void perform(unsigned short int allsets[][2], string input)
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

    if (input == "LIST")
        list(allsets, content);
    else if (input == "SET" && pos != string::npos)
        set(allsets, content);
    else if (input == "SAVE" && pos != string::npos)
        save(allsets, content);
    else if (input == "LOAD" && pos != string::npos)
        load(allsets, content);
    else
        help();

}

void list(unsigned short int allsets[][2], string content)
{
    size_t i(0);
    while (i < 26 && allsets[i][0] != 0)
    {
        cout<<char(allsets[i][0])<<" = ";
        display(allsets[i][1]);
        cout<<endl;
        ++i;
    }
}

void set(unsigned short int allsets[][2], string content)
{
    size_t pos1 = content.find('=');
    char junk;
    size_t storage = 26;

    if (pos1 != string::npos)
    {
        stringstream ss;
        ss << content.substr(0,pos1);
        ss >> junk;
        junk = toupper(junk);
        storage = find(allsets, junk);
        content = content.substr(pos1+1);
    }

    unsigned short int universe = evaluate(allsets, content);

    if (storage < 26)
    {
        cout<< junk<<" = ";
        display(universe);
        allsets[storage][0] = junk;
        allsets[storage][1] = universe;
        cout<<endl;
    }
    else
    {
        display(universe);
        cout<<endl;
    }
}

unsigned short int setUnion(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    third = first | second;

    return third;
}

unsigned short int setIntersection(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    third = first & second;
    return third;
}

unsigned short int setDifference(unsigned short int first, unsigned short int second)
{
    unsigned short int third = 0;
    third = first & ~second;
    return third;
}

void evaluate_vects (vector<unsigned short int> &operands, vector<char> &operators)
{
    unsigned short int second = operands.back();
    operands.pop_back();
    unsigned short int first = operands.back();
    operands.pop_back();

    switch(operators.back())
    {
    case '+':
        operands.push_back(setUnion(first, second));
        break;

    case '*':
        operands.push_back(setIntersection(first, second));
        break;

    case '/':
        operands.push_back(setDifference(first, second));
        break;
    }
    operators.pop_back();
}

unsigned short int evaluate (const unsigned short int allsets[][2], string content)
{
    vector<unsigned short int> operands;
    vector<char> operators;
    stringstream ss;


    replaceBrackets(allsets, content);
    replaceLetters(allsets, content);

    ss << content;

    while(ss && ss.peek() != '\n')
    {
        if (isdigit(ss.peek()))
        {
            unsigned short int number;
            ss >> number;
            operands.push_back(number);
        }
        else if (strchr("+*/", ss.peek()) != NULL)
        {
            char symbol;
            ss >> symbol;
            operators.push_back(symbol);
        }
        else
            ss.ignore();

        if (operands.size() == 2)
            evaluate_vects(operands, operators);
    }

    return operands.back();
}

void replaceBrackets(const unsigned short int allsets[][2], string &expression)
{
    size_t pos1 = expression.find('{');
    while (pos1 != string::npos)
    {
        stringstream ss;
        size_t pos2 = expression.find('}', pos1 + 1);
        if (expression[pos1-1] != '~')
        ss << expression.substr(0,pos1) <<' '<< setFromList(expression.substr(pos1+1, pos2)) << ' ' << expression.substr(pos2+1);
        else
        {
            unsigned short int universe = setFromList(expression.substr(pos1+1, pos2));
            unsigned short int notuniverse = ~universe;
            ss << expression.substr(0,pos1-1) <<' '<< notuniverse << ' ' << expression.substr(pos2+1);

        }

        getline(ss, expression);
        pos1 = expression.find('{');
    }
}

unsigned short int setFromList(string content)
{
    stringstream ss;
    unsigned short int universe = 0;
    ss << content;

    while (ss && ss.peek() != '\n')
    {
        if (isdigit(ss.peek()))
        {
            size_t number;
            ss >> number;
            include(universe,number);
        }
        else
            ss.ignore();
    }

    return universe;
}

void replaceLetters(const unsigned short int allsets[][2], string &expression)
{
    size_t pos = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    while (pos != string::npos)
    {
        stringstream ss;
        if (expression[pos-1] != '~')
            ss<<expression.substr(0,pos)<<' '<<allsets[find(allsets, toupper(expression[pos]))][1]<<' '<<expression.substr(pos+1);
        else
        {
            unsigned short int universe = allsets[find(allsets, toupper(expression[pos]))][1];
            unsigned short int notuniverse = ~universe;
            ss<<expression.substr(0,pos-1)<<' '<<notuniverse<<' '<<expression.substr(pos+1);
        }
        getline(ss, expression);
        pos = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    }
}

void save(const unsigned short int allsets[][2], string filename)
{
    for (size_t i = 0; i < filename.length(); ++i)
        filename[i]=tolower(filename[i]);
    size_t pos = filename.find('.');
    if (pos != string::npos)
        filename = filename.substr(0,pos);
    filename += ".dat";
    ifstream ins(filename.c_str());
    while (! ins.fail())
    {
        ins.close();
        string response;
        cout<<"File already exists. Overwrite? ";
        getline(cin, response);
        if (toupper(response[0]) == 'Y')
           break;
        cout<<"Enter filename: ";
        getline(cin, filename);
        for (size_t i = 0; i < filename.length(); ++i)
            filename[i]=tolower(filename[i]);
        pos = filename.find('.');
        if (pos != string::npos)
            filename = filename.substr(0,pos);
        filename += ".dat";
        ins.open(filename.c_str());
    }
    ins.close();
    ofstream out(filename.c_str());

    size_t i(0);
    while (i < 26 && allsets[i][0] != 0)
    {
        out<<allsets[i][0]<<" "<<allsets[i][1]<<endl;
        ++i;
    }
    out.close();
    cout<<"Saved to "<<filename<<endl;
}

void load(unsigned short int allsets[][2], string filename)
{
    for (size_t i = 0; i < filename.length(); ++i)
        filename[i]=tolower(filename[i]);
    size_t pos = filename.find('.');
    if (pos != string::npos)
        filename = filename.substr(0,pos);
    filename += ".dat";
    ifstream ins(filename.c_str());
    if (ins.fail())
    {
        cout<<"No such file!"<<endl;
        return;
    }
    initialize(allsets);

    string line;
    for (size_t i = 0; i < 26 ; ++i)
    {
        ins >> allsets[i][0];
        ins >> allsets[i][1];
        if (ins.peek() == EOF)
            break;
    }

    ins.close();
    cout<<"File successfully loaded."<<endl;
    list(allsets, "");

}
