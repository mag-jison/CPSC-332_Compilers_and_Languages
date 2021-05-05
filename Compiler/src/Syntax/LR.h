#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>
#include "../Tables/symbol.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define types table.front().types

using namespace std;

typedef queue<Token> symbol;
typedef vector<vector<string>> op;

class LR {
private:
    ofstream output;
    stack<string> ss;
    vector<string> rhs;

    symbol table;

    op parser {{},
               {}};

public:
    LR(){
    }

    void copy(symbol);
    void driver();
};

void LR::copy(symbol temp){

}

void LR::driver(){

}