#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef vector<vector<int>> production;
typedef queue<Token> symbol;

class Stack {
private:
    ofstream output;
    stack<string> ss;
    symbol table;

    string t; // current top of stack
    int r;    // production rule index

    production rules

        // %  ;  ,  b  i  f  =  +  -  *  /  (  ) id integer real if while begin then else endif do whileend end < <= == <> >= >
        {{ 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 3, 0, 1, 3, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // S 
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // A
         { 9, 9, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 9, 9, 0, 0, 0, 0, 0, 0 },  // D
         { 0, 0, 0,10,11,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // TY
         { 0,14,13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // MID
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // E
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16, 0,16,16,16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // T
         {19,19, 0, 0, 0, 0, 0,17,18, 0, 0, 0,19, 0, 0, 0, 0, 0, 0,19,19,19,19,19,19,19,19,19,19,19,19 },  // Q
         {22,22, 0, 0, 0, 0, 0,22,22,20,21, 0,22, 0, 0, 0, 0, 0, 0,22,22,22,22,22,22,22,22,22,22,22,22 },  // R
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,23, 0,24,25,25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // F
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // ID
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27,28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // NUM
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // IF
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // WHILE
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // BEGIN
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,32, 0,32,32,32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // C
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,34, 0, 0,34, 0, 0,33,33,33,33,33,33 },  // B
         { 0,35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,36, 0, 0, 0, 0, 0, 0 },  // MS
         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,37,38,39,40,41,42 }}; // RELOP

public:
    Stack(){
        ss.push("%");
        ss.push("S");
    }

    void copy(symbol&);
    void STACK(symbol);
    bool IS_TERMINAL(string);
    int TERMINAL(string);
    int NONTERMINAL(string);
    void debug(stack<string>);
};

void Stack::debug(stack<string> temp){
    cout << "[ ";
    while (!temp.empty()){
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << "]\n";
}

void Stack::copy(symbol& w) {
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

bool Stack::IS_TERMINAL(string x){
    return (x == ";" || x == "," || x == "bool" || x == "int" || x == "float" || x == "=" || x == "+" || x == "-" || x == "*" || x == "/" || x == "(" || x == ")" | x == "a" || x == "b" || x == "c" || x == "1" || x == "2" || x == "3" || x == "5.5" || x == "if" || x == "then" || x == "else" || x == "endif" || x == "while" || x == "do" || x == "whileend" || x == "begin" || x == "end" || x == "<" || x == "<=" || x == "==" || x == "<>" || x == ">=" || x == ">") ? true : false;
}

int Stack::TERMINAL(string x){
    if (x == "%")
        return 0;
    else if (x == ";")
        return 1;
    else if (x == ",")
        return 2;
    else if (x == "bool")
        return 3;
    else if (x == "int")
        return 4;
    else if (x == "float")
        return 5;
    else if (x == "=")
        return 6;
    else if (x == "+")
        return 7;
    else if (x == "-")
        return 8;
    else if (x == "*")
        return 9;
    else if (x == "/")
        return 10;
    else if (x == "(")
        return 11;
    else if (x == ")")
        return 12;
    else if (x == "a" || x == "b" || x == "c")
        return 13;
    else if (x == "1" || x == "2" || x == "3")
        return 14;
    else if (x == "5.5")
        return 15;
    else if (x == "if")
        return 16;
    else if (x == "while")
        return 17;
    else if (x == "begin")
        return 18;
    else if (x == "then")
        return 19;
    else if (x == "else")
        return 20;
    else if (x == "endif")
        return 21;
    else if (x == "do")
        return 22;
    else if (x == "whileend")
        return 23;
    else if (x == "end")
        return 24;
    else if (x == "<")
        return 25;
    else if (x == "<=")
        return 26;
    else if (x == "==")
        return 27;
    else if (x == "<>")
        return 28;
    else if (x == ">=")
        return 29;
    else if (x == ">")
        return 30;
    else
        return -1;
}

int Stack::NONTERMINAL(string x){
    if (x == "S")
        return 0;
    else if (x == "A")
        return 1;
    else if (x == "D")
        return 2;
    else if (x == "TY")
        return 3;
    else if (x == "MID")
        return 4;
    else if (x == "E")
        return 5;
    else if (x == "T")
        return 6;
    else if (x == "Q")
        return 7;
    else if (x == "R")
        return 8;
    else if (x == "F")
        return 9;
    else if (x == "ID")
        return 10;
    else if (x == "NUM")
        return 11;
    else if (x == "IF")
        return 12;
    else if (x == "WHILE")
        return 13;
    else if (x == "BEGIN")
        return 14;
    else if (x == "C")
        return 15;
    else if (x == "B")
        return 16;
    else if (x == "MS")
        return 17;
    else if (x == "RELOP")
        return 18;
    else
        return -1;
}

void Stack::STACK(symbol w) {
    system("cls");
    output.open("out.txt");
    copy(w);
    cout << "STACK PRODUCTION\n----------------\n\n";

    while (ss.top() != "%") {
        t = ss.top();
        debug(ss);
        if (IS_TERMINAL(t)){
            if (t == lexeme){
                ss.pop();
                table.pop();
            }
            else {
                output << "\n***NOT ACCEPTED terminal***\n";
                output.close();
                exit(0);
            }
        }
        else {
            ss.pop();
            r = rules[NONTERMINAL(t)][TERMINAL(lexeme)];
            if (r != 0){
                switch (r){
                    case 1:
                        output << write << "<S> -> <A>" << endl;
                        ss.push("A");
                        break;
                    case 2:
                        output << write << "<S> -> <D>" << endl;
                        ss.push("D");
                        break;
                    case 3:
                        output << write << "<S> -> <E>" << endl;
                        ss.push("E");
                        break;
                    case 4:
                        output << write << "<S> -> <IF>" << endl;
                        ss.push("IF");
                        break;
                    case 5:
                        output << write << "<S> -> <WHILE>" << endl;
                        ss.push("WHILE");
                        break;
                    case 6:
                        output << write << "<S> -> <BEGIN>" << endl;
                        ss.push("BEGIN");
                        break;
                    case 7: 
                        output << "<A> -> <ID> = <E>" << endl;   
                        ss.push("E");
                        ss.push("=");
                        ss.push("ID");
                        break;
                    case 8:
                        output << "<D> -> <TY> <ID> <MORE_IDS>" << endl;
                        ss.push("MID");
                        ss.push("ID");
                        ss.push("TY");
                        break;
                    case 9:
                        output << "<D> -> <empty>" << endl;
                        break;
                    case 10:
                        output << "<TY> -> bool" << endl;
                        ss.push("bool");
                        break;
                    case 11: 
                        output << "<TY> -> int" << endl;
                        ss.push("int");
                        break;
                    case 12:
                        output << "<TY> -> float" << endl;
                        ss.push("float");
                        break;
                    case 13: 
                        output << write << "<MORE_IDS> -> , <ID> <MORE_IDS>\n" << write;
                        ss.push("MID");
                        ss.push("ID");
                        ss.push(",");
                        break;
                    case 14:
                        output << "<MORE_IDS> -> <empty>" << endl;
                        break;
                    case 15:
                        output << write << "<E> -> <T> <Q>" << endl;
                        ss.push("Q");
                        ss.push("T");
                        break;
                    case 16:
                        output << "<T> -> <F> <R>" << endl;
                        ss.push("R");
                        ss.push("F");
                        break;
                    case 17:
                        output << write << "<Q> -> + <T> <Q>" << endl;
                        ss.push("Q");
                        ss.push("T");
                        ss.push("+");
                        break;
                    case 18:
                        output << write << "<Q> -> - <T> <Q>" << endl;
                        ss.push("Q");
                        ss.push("T");
                        ss.push("-");
                        break;
                    case 19:
                        output << "<Q> -> <empty>" << endl;
                        break;
                    case 20:
                        output << write << "<R> -> * <F> <R>" << endl;
                        ss.push("R");
                        ss.push("F");
                        ss.push("*");
                        break;
                    case 21: 
                        output << write << "<R> -> / <F> <R>" << endl;
                        ss.push("R");
                        ss.push("F");
                        ss.push("/");
                        break;
                    case 22:
                        output << "<R> -> <empty>" << endl;
                        break;
                    case 23: 
                        output << "<F> -> ( <E> )" << endl;
                        ss.push(")");
                        ss.push("E");
                        ss.push("(");
                        break;
                    case 24:
                        output << "<F> -> <ID>" << endl;
                        ss.push("ID");
                        break;
                    case 25:
                        output << "<F> -> <NUM>" << endl;
                        ss.push("NUM");
                        break;
                    case 26:
                        output << "<ID> -> " << lexeme << endl; 
                        ss.push(lexeme);
                        break;
                    case 27:
                        output << "<NUM> -> " << lexeme << endl; 
                        ss.push(lexeme);
                        break;
                    case 28: 
                        output << "<NUM> -> " << lexeme << endl; 
                        ss.push(lexeme);
                        break;
                    case 29: 
                        output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                        ss.push("endif");
                        ss.push("S");
                        ss.push("else");
                        ss.push("S");
                        ss.push("then");
                        ss.push("C");
                        ss.push("if");
                        break;
                    case 30:
                        output << "<WHILE> -> while <C> do <S> whileend" << endl;
                        ss.push("whileend");
                        ss.push("S");
                        ss.push("do");
                        ss.push("C");
                        ss.push("while");
                        break;
                    case 31:
                        output << "<BEGIN> -> begin <S> <MS> end" << endl;
                        ss.push("end");
                        ss.push("MS");
                        ss.push("S");
                        ss.push("begin");
                        break;
                    case 32:
                        output << "<C> -> <E> <B>" << endl;
                        ss.push("B");
                        ss.push("E");
                        break;
                    case 33: 
                        output << "<B> -> <RELOP> <E>" << endl;
                        ss.push("E");
                        ss.push("RELOP");
                        break;
                    case 34: 
                        output << "<B> -> <empty>" << endl;
                        break;
                    case 35: 
                        output << "<MS> -> ; <S> <MS>" << endl;
                        ss.push("MS");
                        ss.push("S");
                        ss.push(";");
                        break;
                    case 36: 
                        output << "<MS> -> <empty>" << endl;
                        break;
                    case 37: 
                        output << write << "<RELOP> -> <" << endl;
                        ss.push("<");
                        break;
                    case 38: 
                        output << "<RELOP> -> <=" << endl;
                        ss.push("<=");
                        break;
                    case 39: 
                        output << "<RELOP> -> ==" << endl;
                        ss.push("==");
                        break;
                    case 40: 
                        output << "<RELOP> -> <>" << endl;
                        ss.push("<>");
                        break;
                    case 41: 
                        output << "<RELOP> -> >=" << endl;
                        ss.push(">=");
                        break;
                    case 42: 
                        output << "<RELOP> -> >" << endl;
                        ss.push(">");
                        break;
                }
            }
            else {
               output << "\n***NOT ACCEPTED production***\n";
               output.close();
               exit(0);
            }
        }
    }

    cout << "\033[32m" << "\nPASSED: " << "\033[0m" << "view in out.txt\n";
    output << "\n***ACCEPTED***\n";
    output.close();
}
