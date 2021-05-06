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
#define types table.front().types

using namespace std;

typedef queue<Token> symbol;
typedef vector<vector<string>> transition;

class LR {
private:
    ofstream output;
    symbol table;
    transition list;
    stack<string> ss;
    string TOS, X;

public:
    int convert(const string&);
    int convert(Token);
    void copy(symbol);
    void driver();
};

void LR::copy(symbol temp){

}

int LR::convert(const string& w){

}

int LR::convert(Token t){

}

void LR::driver(){
    ss.push("0");

    while (TOS != "ACCT"/* || error*/){
        X = list[convert(TOS), convert(table.front())];

        if (X[0] == 'S'){
            ss.push(lexeme);   // pushing current token
            ss.push(X.back()); // pushing new state
            ss.pop();

        }
        else if (X[0] == 'R'){

        }
        else if (X == "ACCT"){

        }
        else /*error*/
    }
}