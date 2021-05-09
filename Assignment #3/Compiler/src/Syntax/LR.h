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
    string X, TOS, g;
    vector<vector<string>> rtable {{ "S5", "", "", "", "", "S4", "", "", "1", "2", "3"      },
                                   { "", "S6", "S7", "", "", "", "", "ACCT", "", "", ""     },
                                   { "", "R3", "R3", "S8", "S9", "", "R3", "R3", "", "", "" },
                                   { "", "R6", "R6", "R6", "R6", "", "R6", "R6", "", "", "" },
                                   { "S5", "", "", "", "", "S4", "", "", "10", "2", "3"     },
                                   { "", "R8", "R8", "R8", "R8", "", "R8", "R8", "", "", "" },
                                   { "S5", "", "", "", "", "S4", "", "", "", "11", "3"      },
                                   { "S5", "", "", "", "", "S4", "", "", "", "12", "3"      },
                                   { "S5", "", "", "", "", "S4", "", "", "", "", "13"       },
                                   { "S5", "", "", "", "", "S4", "", "", "", "", "14"       },
                                   { "", "S6", "S7", "", "", "", "S15", "", "", "", ""      },
                                   { "", "R1", "R1", "S8", "S9", "", "R1", "R1","", "", ""  },
                                   { "", "R2", "R2", "S8", "S9", "", "R2", "R2", "", "", "" },
                                   { "", "R4", "R4", "R4", "R4", "", "R4", "R4", "", "", "" },
                                   { "", "R5", "R5", "R5", "R5", "", "R5", "R5", "", "", "" },
                                   { "", "R7", "R7", "R7", "R7", "", "R7", "R7", "", "", "" }};
public:
    int rules(const int&);
    string LHS(const string&);
    int to_int(const string&);
    int terminal(const string&);
    void copy(symbol);
    bool driver(symbol);
    void sspace(int);
    void debug(stack<string>, symbol);
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
            output << "E -> E + T, T [ ";
            return 3;
        case 2:
            output << "E -> E - T, T [ ";
            return 3;
        case 3:
            output << "E -> T, T [ ";
            return 1;
        case 4:
            output << "T -> T * F, T [ ";
            return 3;
        case 5:
            output << "T -> T / F, T [ ";
            return 3;
        case 6:
            output << "T -> F, T [ ";
            return 1;
        case 7:
            output << "F -> ( E ), T [ ";
            return 3;
        case 8:
            output << "F -> id, T [ ";
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
    else if (w == "12")
        return 12;
    else if (w == "13")
        return 13;
    else if (w == "14")
        return 14;
    else if (w == "15")
        return 15;
    else
        return -1;
}

int LR::terminal(const string& w){
    if (token == "IDENTIFIER")
        return 0;
    else if (w == "+")
        return 1;
    else if (w == "-")
        return 2;
    else if (w == "*")
        return 3;
    else if (w == "/")
        return 4;
    else if (w == "(")
        return 5;
    else if (w == ")")
        return 6;
    else if (w == "%")
        return 7;
    else if (w == "E")
        return 8;
    else if (w == "T")
        return 9;
    else if (w == "F")
        return 10;
    else
        return -1;
}

string LR::LHS(const string& w){
    if (w == "1")
        return "E";
    else if (w == "2")
        return "E";
    else if (w == "3")
        return "E";
    else if (w == "4")
        return "T";
    else if (w == "5")
        return "T";
    else if (w == "6")
        return "T";
    else if (w == "7")
        return "F";
    else if (w == "8")
        return "F";
    else
        return "";
}

void LR::sspace(int num){
    output << g;
    for (int i = 0; i < num-(g.length()); ++i)
        output << " ";
}

void LR::debug(stack<string> temp, symbol temp2){
    g = "";
    while (!temp.empty()){
        g = temp.top() + g;
        // INSERT TO THE BACK
        temp.pop();
    }
    g = "%" + g;
    sspace(20);
    output << "\t\t";
    g = "";
    while (!temp2.empty()){
        g += temp2.front().lex;
        temp2.pop();
    }
    sspace(20);
    output << "\t\t";
}

bool LR::driver(symbol temp){
    system("cls");
    ss.push("0");
    output.open("files/out.txt");
    output << "+--START--+\n\n";
    copy(temp);

    while (ss.top() != "ACCT"){
        debug(ss, table);
        TOS = ss.top();
        X = rtable[to_int(TOS)][terminal(lexeme)];
        // indexes top of stack and current input token
        output << "T = [ " << to_int(TOS) << ", " << lexeme << " ]\t\t" << X << "\t\t";

        if (X[0] == 'S'){
            output << "push( " << lexeme << " ), ";
            ss.push(lexeme);      // pushing current input token
            output << "push( " << X.substr(1) << " )\n";
            ss.push(X.substr(1)); // pushing new state (string)
            table.pop();
        }
        else if (X[0] == 'R'){
            int max = rules(to_int(X.substr(1)));
            for (int i = 0; i < (2*max); ++i)
                ss.pop(); // pops two times amount of elements on the RHS production rule
            TOS = ss.top();
            output << TOS << ", " << LHS(X.substr(1)) << " ]\n";
            ss.push(LHS(X.substr(1))); // new nonterminal gets pushed on to the stack
            ss.push(rtable[to_int(TOS)][terminal(ss.top())]); // indexes table to get the next state
        }
        else if (X == "ACCT")
            ss.push("ACCT");
        else
            ss.push("ERROR");
    }
    output << "\n\n+--FINISH--+";
    output.close();
    return (ss.top() == "ACCT") ? true : false;
}
