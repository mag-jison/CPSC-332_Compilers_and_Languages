#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>
#include "lexer.h"
#include "error_table.h"

#define token w.front().first
#define lexeme w.front().second

using namespace std;

typedef queue<pair<string, string>> symbol;

class Syntax {
private:
    ofstream output;
    string g, e;
    symbol temp;

public:
};

/* Wait until I am finish and then you can copy */
