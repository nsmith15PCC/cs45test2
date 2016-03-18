#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

static const string COLORS[16] = {"WHITE", "GOLD", "GREY", "RED", "GREEN", "BLUE", "ORANGE", "PURPLE", "YELLOW", "VIOLET", "MAGENTA", "CYAN", "RUST", "NAVY", "BURGUNDY", "BLACK"};
static const char OPERATIONS[6] = {'+', '*', '\\', '<', '>', '='};
static const string COMMANDS[7] = {"SET","IS", "LIST", "SHOW", "SAVE", "LOAD", "HELP"};


void include(unsigned short int &universe, size_t who);
void display(unsigned short int universe);
size_t find(const unsigned short int allsets[][2], char tofind);
void initialize(unsigned short int allsets[][2]);
void help();
void perform(unsigned short int allsets[][2], string input);
void list(unsigned short int allsets[][2], string content);
void show (unsigned short int allsets[][2], string content);
void set(unsigned short int allsets[][2], string content);
void is(unsigned short int allsets[][2], string content);
unsigned short int setUnion(unsigned short int first, unsigned short int second);
unsigned short int setIntersection(unsigned short int first, unsigned short int second);
unsigned short int setDifference(unsigned short int first, unsigned short int second);
void evaluate_vects (vector<unsigned short int> &operands, vector<char> &operators);
unsigned short int evaluate (const unsigned short int allsets[][2], string content);
void replaceColors (string &expression);
void replaceLetters(const unsigned short int allsets[][2], string &expression);
unsigned short int setFromList(string content);
void replaceBrackets(const unsigned short int allsets[][2], string &expression);
void save(const unsigned short int allsets[][2], string filename);
void load(unsigned short int allsets[][2], string filename);


int main()
{
    unsigned short int allsets[52][2];
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
    bool notempty = false;
    cout<<"{";
    for(unsigned short int i = 0; i < 16; ++i)
    {
        unsigned short int result;
        result = (mask << i) & universe;
        if(result)
        {
            notempty = true;
            cout<<COLORS[i]<<", ";
        }
    }
    if (notempty)
    cout<<"\b\b}";
    else
        cout<<"\bNULL";
}

size_t find(const unsigned short int allsets[][2], char tofind)
{
    size_t i = 0;
    while (i < 52 && allsets[i][0] != 0 && allsets[i][0] != int(tofind))
        ++i;
    return i;
}

void initialize(unsigned short int allsets[][2])
{
    for (size_t i = 0; i < 52; ++i)
        for (size_t j = 0; j < 2 ; ++j)
            allsets[i][j] = 0;
    return;
}

void help()
{
    cout<<"Commands:"<<endl
       <<"LIST - List all sets and their contents"<<endl
      <<"SHOW <set> - List all sets and their contents"<<endl

      <<"HELP - Give help"<<endl
     <<"SET - Enter a set (can be a definition, union, intersection, or combination of these)"<<endl
    <<"    <set1> '+' <set2> - Returns the union of two sets"<<endl
    <<"    <set1> '*' <set2> - Returns the intersection of two sets"<<endl
    <<"    <set1> '\\' <set2> - Returns the difference of two sets"<<endl
    <<"IS - Allows the following three set comparisons"<<endl
    <<"    <set1> '<' <set2> - Returns true if the first set is a subset of the second"<<endl
    <<"    <set1> '>' <set2> - Returns true if the first set is a superset of the second"<<endl
    <<"    <set1> '=' <set2> - Returns true if two sets are equivalent"<<endl
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

    size_t command;
    for (command = 0; command < 6 && input != COMMANDS[command]; command++);

switch (command)
{
case (0):
    set(allsets, content);
    break;

case (1):
    is(allsets, content);
    break;

case (2):
    list(allsets, content);
    break;

case(3):
    show(allsets, content);
    break;

case(4):
    save(allsets, content);
    break;

case(5):
    load(allsets, content);
    break;

case (6):
    cout<<"Invalid command."<<endl;
    help();
    break;

default:
    help();

}

}

void list(unsigned short int allsets[][2], string content)
{
    size_t i(0);
    while (i < 52 && allsets[i][0] != 0)
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
    size_t storage = 52;

    if (pos1 != string::npos)
    {
        stringstream ss;
        ss << content.substr(0,pos1);
        ss >> junk;
        storage = find(allsets, junk);
        content = content.substr(pos1+1);
    }

    unsigned short int universe = evaluate(allsets, content);

    if (storage < 52)
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


void is(unsigned short int allsets[][2], string content)
{
    stringstream ss;
    unsigned short int first, second;
    char op;
    bool result;



    replaceColors(content);
    replaceBrackets(allsets, content);
    replaceLetters(allsets, content);

    ss << content;

    ss >> first >> op >> second;

    switch (op)
    {
    case '<':
        result = ((first | second) == second);
                break;

    case '>':
        result = ((first | second) == first);
        break;

    case '=':
        result = (first == second);
        break;

    default:
        cout<<"Invalid syntax!"<<endl;
        return;
    }

    cout << (result ? "TRUE" : "FALSE" )<<endl;

}


void show (unsigned short int allsets[][2], string content)
{
    stringstream ss(content);
    char index;
    ss >> index;
    cout << index << " = ";
    display(allsets[find(allsets, index)][1]);
    cout <<endl;
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

    case '\\':
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

    replaceColors(content);
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
        else if (strchr("+*\\", ss.peek()) != NULL)
        {
            char symbol;
            ss >> symbol;
            operators.push_back(symbol);
            if (operators.size() > 0 || operands.size() < 1)
            {
                cout << "Invalid expression!"<<endl;
                return 0;
            }
        }


        else
            ss.ignore();

        if (operands.size() == 2)
            evaluate_vects(operands, operators);
    }

    if (operands.size() == 1)
    return operands.back();
    else
    {
        cout << "Invalid expression!"<<endl;
        return 0;
    }
}

void replaceColors (string & expression)
{
size_t pos1 = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

while (pos1 < expression.length()-1)
{
    size_t pos2 = expression.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", pos1);
    if (pos2 != pos1+1)
    {
        stringstream ss;
        string color = expression.substr(pos1, pos2-pos1);
        for (size_t i = 0; i < color.size(); ++i)
            color[i] = toupper(color[i]);
        size_t number;
        for (number = 0; number < 16 && color != COLORS[number]; ++number);
        ss << expression.substr(0,pos1) << number << expression.substr(pos2);
        getline(ss, expression);
    }
    pos1 = expression.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", pos1 + 1);
}
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
            ss<<expression.substr(0,pos)<<' '<<allsets[find(allsets, expression[pos])][1]<<' '<<expression.substr(pos+1);
        else
        {
            unsigned short int universe = allsets[find(allsets, expression[pos])][1];
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
    while (i < 52 && allsets[i][0] != 0)
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
    for (size_t i = 0; i < 52 ; ++i)
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
