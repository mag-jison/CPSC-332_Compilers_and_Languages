#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>
#include "symbol.h"
#include "error.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define types table.front().types

using namespace std;

typedef queue<Token> symbol;

class LR : public SymbolTable, public ErrorTable {
private:
    ofstream output;
    symbol table;
    vector<vector<string>> rtable {{"S5", "", "", "S4", "", "", "1", "2", "3"},
                                   {"", "S6", "", "", "", "ACCT", "", "", ""},
                                   {"", "R2", "S7", "", "R2", "R2", "", "", ""},
                                   {"", "R4", "R4", "", "R4", "R4", "", "", ""},
                                   {"S5", "", "", "S4", "", "", "8", "2", "3"},
                                   {"", "R6", "R6", "", "R6", "R6", "", "", ""},
                                   {"S5", "", "", "S4", "", "", "", "9", "3"},
                                   {"S5", "", "", "S4", "", "", "", "", "10"},
                                   {"", "S6", "", "", "S11", "", "", "", ""},
                                   {"", "R1", "S7", "", "R1", "R1", "", "",""},
                                   {"", "R3", "R3", "", "R3", "R3", "", "", ""},
                                   {"", "R5", "R5", "", "R5", "R5", "", "", ""}};

    stack<string> ss;
    string X, TOS;

public:
    LR(string f){
        sfile = f;
        efile = f;
    }

    int rules(const int&);
    string LHS(const string&);
    int to_int(const string&);
    int terminal(const string&);
    void copy(symbol);
    bool driver(symbol table);
};

void LR::copy(symbol w){
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

int LR::rules(const int& i){
    switch (i){
        case 1:
            return 6;
        case 2:
            return 2;
        case 3:
            return 6;
        case 4:
            return 2;
        case 5:
            return 6;
        case 6:
            return 2;
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
    ss.push("0");
    copy(temp);

    while (ss.top() != "ACCT"){
        TOS = ss.top();
        X = rtable[to_int(TOS)][terminal(lexeme)];
        cout << "T = [ " << to_int(TOS) << ", " << terminal(lexeme) << " ]" << endl;
        // indexes top of stack && current input token

        if (X[0] == 'S'){
            ss.push(lexeme);      // pushing current input token
            ss.push(X.substr(1)); // pushing new state (string)
            table.pop();
        }
        else if (X[0] == 'R'){
            for (int i = 0; i < rules(to_int(X.substr(1))); ++i)
                ss.pop();
            TOS = ss.top();
            ss.push(LHS(X.substr(1)));
            ss.push(rtable[to_int(TOS)][terminal(ss.top())]);
        }
        else if (X == "ACCT")
            return true;
        else {
            cout << "ERROR\n";
            system("pause");
        }
    }
}



/*
 * table = full of strings
 * "" = invalid entry
 * "ACCT" = accepted string, ends processing
 * S# = shift by rule #
 * R# = reduce by rule #
 */
