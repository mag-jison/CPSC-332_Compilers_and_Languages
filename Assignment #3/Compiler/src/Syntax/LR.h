/*
 * table = full of strings
 * "" = invalid entry
 * "ACCT" = accepted string, ends processing
 * S# = shift by rule #
 * R# = reduce by rule #
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>
#include "../Tables/symbol.h"
#include "../Tables/error.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define types table.front().type

using namespace std;

typedef queue<Token> symbol;

class LR : public SymbolTable, public ErrorTable {
private:
    ofstream output;
    symbol table;
    stack<string> ss;
    string X, TOS;
/*
 * E -> E + T | T
 * T -> T * F | F
 * F -> ( E ) | id
*/
//                                    id   +   *    (    )   %    E    T    F
    vector<vector<string>> rtable {{ "S5", "", "", "S4", "", "", "1", "2", "3"  },  // 0
                                   { "", "S6", "", "", "", "ACCT", "", "", ""   },  // 1
                                   { "", "R2", "S7", "", "R2", "R2", "", "", "" },  // 2
                                   { "", "R4", "R4", "", "R4", "R4", "", "", "" },  // 3
                                   { "S5", "", "", "S4", "", "", "8", "2", "3"  },  // 4
                                   { "", "R6", "R6", "", "R6", "R6", "", "", "" },  // 5
                                   { "S5", "", "", "S4", "", "", "", "9", "3"   },  // 6
                                   { "S5", "", "", "S4", "", "", "", "", "10"   },  // 7
                                   { "", "S6", "", "", "S11", "", "", "", ""    },  // 8
                                   { "", "R1", "S7", "", "R1", "R1", "", "",""  },  // 9
                                   { "", "R3", "R3", "", "R3", "R3", "", "", "" },  // 10
                                   { "", "R5", "R5", "", "R5", "R5", "", "", "" }}; // 11
public:
    int rules(const int&);
    string LHS(const string&);
    int to_int(const string&);
    int terminal(const string&);
    void copy(symbol);
    bool driver(symbol);
};

void LR::copy(symbol w){
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

int LR::rules(const int& i){ // returns the number of RHS productions
    switch (i){
        case 1:
            return 3;
        case 2:
            return 1;
        case 3:
            return 3;
        case 4:
            return 1;
        case 5:
            return 3;
        case 6:
            return 1;
        default:
            return -1;
    }
}

int LR::to_int(const string& w){
    if (w == "0")
        return 0;
    else if (w == "1")
        return 1;
    else if (w == "2")
        return 2;
    else if (w == "3")
        return 3;
    else if (w == "4")
        return 4;
    else if (w == "5")
        return 5;
    else if (w == "6")
        return 6;
    else if (w == "7")
        return 7;
    else if (w == "8")
        return 8;
    else if (w == "9")
        return 9;
    else if (w == "10")
        return 10;
    else if (w == "11")
        return 11;
    else
        return -1;
}

int LR::terminal(const string& w){
    if (token == "IDENTIFIER")
        return 0;
    else if (w == "+")
        return 1;
    else if (w == "*")
        return 2;
    else if (w == "(")
        return 3;
    else if (w == ")")
        return 4;
    else if (w == "%")
        return 5;
    else if (w == "E")
        return 6;
    else if (w == "T")
        return 7;
    else if (w == "F")
        return 8;
    else
        return -1;
}

string LR::LHS(const string& w){
    if (w == "1")
        return "E";
    else if (w == "2")
        return "E";
    else if (w == "3")
        return "T";
    else if (w == "4")
        return "T";
    else if (w == "5")
        return "F";
    else if (w == "6")
        return "F";
    else
        return "";
}

bool LR::driver(symbol temp){
    system("cls");
    ss.push("0");
    output.open("files/out.txt");
    copy(temp);

    while (ss.top() != "ACCT"){
        TOS = ss.top();
        X = rtable[to_int(TOS)][terminal(lexeme)];
        // indexes top of stack and current input token
        output << "T = [ " << to_int(TOS) << ", " << terminal(lexeme) << " ]\n";

        if (X[0] == 'S'){
            ss.push(lexeme);      // pushing current input token
            ss.push(X.substr(1)); // pushing new state (string)
            table.pop();
        }
        else if (X[0] == 'R'){
            for (int i = 0; i < (2*rules(to_int(X.substr(1)))); ++i)
                ss.pop(); // pops two times amount of elements on the RHS production rule
            TOS = ss.top();
            ss.push(LHS(X.substr(1))); // new nonterminal gets pushed on to the stack
            ss.push(rtable[to_int(TOS)][terminal(ss.top())]); // indexes table to get the next state
        }
        else if (X == "ACCT")
            return true;
        else {
            cout << "ERROR\n";
            return false;
        }
    }
}
