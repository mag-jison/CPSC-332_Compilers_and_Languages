#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>
#include "../Tables/tree.h"
#include "../Tables/symbol.h"
#include "../Tables/error.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define types table.front().ttype
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef vector<vector<int>> production;
typedef queue<Token> symbol;

class STACK : public SymbolTable, public ErrorTable, public Tree<string> {
private:
    ofstream output;
    stack<string> ss;
    bool on;

    string t;
    size_t r;

    symbol table;
    production rules {{ 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 3, 0, 1, 3, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 9, 9, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 0, 9, 9, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0,10,11,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0,14,13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16, 0,16,16,16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      {19,19, 0, 0, 0, 0, 0,17,18, 0, 0, 0,19, 0, 0, 0, 0, 0, 0,19,19,19,19,19,19,19,19,19,19,19,19 },
                      {22,22, 0, 0, 0, 0, 0,22,22,20,21, 0,22, 0, 0, 0, 0, 0, 0,22,22,22,22,22,22,22,22,22,22,22,22 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,23, 0,24,25,25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,27,28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,32, 0,32,32,32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,34, 0, 0,34, 0, 0,33,33,33,33,33,33 },
                      { 0,35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,36, 0, 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,37,38,39,40,41,42 }};

public:
    STACK(){
        ss.push("%");
        ss.push("S");
    }

    Node<string>* getRoot();
    void getTree();
    void debug(stack<string>, symbol);
    void copy(symbol&);
    bool parser(symbol, bool);
    bool IS_TERMINAL(string);
    int TERMINAL(string);
    int NONTERMINAL(string);
};

void STACK::getTree(){
    cout << "+--Parse Tree--+\n\n";
    printTree();
    cout << "\n+--------------+\n";
}

Node<string>* STACK::getRoot(){
    return getRoot();
}

void STACK::debug(stack<string> temp, symbol table){
    string g = "";
    cout << "[ ";
    while (!temp.empty()){
        g = g + temp.top() + " ";
        temp.pop();
    }
    cout << g << "]" << setw(35-g.length()) << "|";
    g = "";
    while (!table.empty()){
        g += lexeme;
        table.pop();
    }
    cout << setw(20) << g << setw(35-g.length());
}

void STACK::copy(symbol& w) {
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

bool STACK::IS_TERMINAL(string x){
    return (x == "%" || x == ";" || x == "," || x == "bool" || x == "int" || x == "float" || x == "=" || x == "+" || x == "-" || x == "*" || x == "/" || x == "(" || x == ")" | x == "a" || x == "b" || x == "c" || x == "1" || x == "2" || x == "3" || x == "5.5" || x == "if" || x == "then" || x == "else" || x == "endif" || x == "while" || x == "do" || x == "whileend" || x == "begin" || x == "end" || x == "<" || x == "<=" || x == "==" || x == "<>" || x == ">=" || x == ">") ? true : false;
}

int STACK::TERMINAL(string x){
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

int STACK::NONTERMINAL(string x){
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

bool STACK::parser(symbol w, bool b){ 
    system("cls");
    copy(w);
    on = b;
    output.open("files/out.txt");
    output << "+--START--+\n\n";
    cout << "STACK | PRODUCTION | ACTION\n---------------------------\n";

    while (!ss.empty()) {
        t = ss.top();
        debug(ss, table);
        if (IS_TERMINAL(t)){
            if (t == lexeme){
                cout << setw(20) << "| pop(" << t << "), lexer()";
                ss.pop();
                table.pop();
            }
            else {
                output << "\n***NOT ACCEPTED***\n";
                output.close();
                return false;
            }
        }
        else {
            ss.pop();
            r = rules[NONTERMINAL(t)][TERMINAL(lexeme)];
            switch (r){
                case 1:
                    if (on)
                        output << "<S> -> <A>" << endl;
                    storeTree("S", vector<string> { "A" });
                    cout << setw(20) << " | pop(" << t << "), push(A)";
                    ss.push("A");
                    break;
                case 2:
                    if (on)
                        output << "<S> -> <D>" << endl;
                    storeTree("S", vector<string> { "D" });
                    cout << setw(20) << "| pop(" << t << "), push(D)";
                    ss.push("D");
                    break;
                case 3:
                    if (on)
                        output << "<S> -> <E>" << endl;
                    storeTree("S", vector<string> { "E" });
                    cout << setw(20) << "| pop(" << t << "), push(E)";
                    ss.push("E");
                    break;
                case 4:
                    if (on)
                        output << "<S> -> <IF>" << endl;
                    storeTree("S", vector<string> { "IF" });
                    cout << setw(20) << "| pop(" << t << "), push(IF)";
                    ss.push("IF");
                    break;
                case 5:
                    if (on)
                        output << "<S> -> <WHILE>" << endl;
                    storeTree("S", vector<string> { "WHILE" });
                    cout << setw(20) << "| pop(" << t << "), push(WHILE)";
                    ss.push("WHILE");
                    break;
                case 6:
                    if (on)
                        output << "<S> -> <BEGIN>" << endl;
                    storeTree("S", vector<string> { "BEGIN" });
                    cout << setw(20) << "| pop(" << t << "), push(BEGIN)";
                    ss.push("BEGIN");
                    break;
                case 7:
                    if (on)
                        output << "<A> -> <ID> = <E>" << endl;
                    storeTree("A", vector<string> { "ID", "=", "E" });
                    cout << setw(20) << "| pop(" << t << "), push(E = ID)";
                    ss.push("E");
                    ss.push("=");
                    ss.push("ID");
                    break;
                case 8:
                    if (on)
                        output << "<D> -> <TY> <ID> <MORE_IDS> ;" << endl;
                    storeTree("D", vector<string> { "TY", "ID", "MID", ";" });
                    cout << setw(20) << "| pop(" << t << "), push(; MID ID TY)";
                    ss.push(";");
                    ss.push("MID");
                    ss.push("ID");
                    ss.push("TY");
                    break;
                case 9:
                    if (on)
                        output << "<D> -> <empty>" << endl;
                    storeTree("D", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 10:
                    if (on)
                        output << "<TY> -> bool" << endl;
                    storeTree("TY", vector<string> { "bool" });
                    cout << setw(20) << "| pop(" << t << "), push(bool)";
                    ss.push("bool");
                    break;
                case 11:
                    if (on)
                        output << "<TY> -> int" << endl;
                    storeTree("TY", vector<string> { "int" });
                    cout << setw(20) << "| pop(" << t << "), push(int)";
                    ss.push("int");
                    break;
                case 12:
                    if (on)
                        output << "<TY> -> float" << endl;
                    storeTree("TY", vector<string> { "float" });
                    cout << setw(20) << "| pop(" << t << "), push(float)";
                    ss.push("float");
                    break;
                case 13:
                    if (on)
                        output << "<MORE_IDS> -> , <ID> <MORE_IDS>\n";
                    storeTree("MID", vector<string> { ",", "ID", "MID" });
                    cout << setw(20) << "| pop(" << t << "), push(MID ID ,)";
                    ss.push("MID");
                    ss.push("ID");
                    ss.push(",");
                    break;
                case 14:
                    if (on)
                        output << "<MORE_IDS> -> <empty>" << endl;
                    storeTree("MID", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 15:
                    if (on)
                        output << "<E> -> <T> <Q>" << endl;
                    storeTree("E", vector<string> { "T", "E" });
                    cout << setw(20) << "| pop(" << t << "), push(Q T)";
                    ss.push("Q");
                    ss.push("T");
                    break;
                case 16:
                    if (on)
                        output << "<T> -> <F> <R>" << endl;
                    storeTree("T", vector<string> { "F", "R" });
                    cout << setw(20) << "| pop(" << t << "), push(R F)";
                    ss.push("R");
                    ss.push("F");
                    break;
                case 17:
                    if (on)
                        output << "<Q> -> + <T> <Q>" << endl;
                    storeTree("Q", vector<string> { "+", "T", "Q" });
                    cout << setw(20) << "| pop(" << t << "), push(Q T +)";
                    ss.push("Q");
                    ss.push("T");
                    ss.push("+");
                    break;
                case 18:
                    if (on)
                        output << "<Q> -> - <T> <Q>" << endl;
                    storeTree("Q", vector<string> { "-", "T", "Q" });
                    cout << setw(20) << "| pop(" << t << "), push(Q T -)";
                    ss.push("Q");
                    ss.push("T");
                    ss.push("-");
                    break;
                case 19:
                    if (on)
                        output << "<Q> -> <empty>" << endl;
                    storeTree("Q", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 20:
                    if (on)
                        output << "<R> -> * <F> <R>" << endl;
                    storeTree("R", vector<string> { "*", "F", "R" });
                    cout << setw(20) << "| pop(" << t << "), push(R F *)";
                    ss.push("R");
                    ss.push("F");
                    ss.push("*");
                    break;
                case 21:
                    if (on)
                        output << "<R> -> / <F> <R>" << endl;
                    storeTree("R", vector<string> { "/", "F", "R" });
                    cout << setw(20) << "| pop(" << t << "), push(R F /)";
                    ss.push("R");
                    ss.push("F");
                    ss.push("/");
                    break;
                case 22:
                    if (on)
                        output << "<R> -> <empty>" << endl;
                    storeTree("R", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 23:
                    if (on)
                        output << "<F> -> ( <E> )" << endl;
                    storeTree("F", vector<string> { "(", "E", ")" });
                    cout << setw(20) << "| pop(" << t << "), push( )E( )";
                    ss.push(")");
                    ss.push("E");
                    ss.push("(");
                    break;
                case 24:
                    if (on)
                        output << "<F> -> <ID>" << endl;
                    storeTree("F", vector<string> { "ID" });
                    cout << setw(20) << "| pop(" << t << "), push(ID)";
                    ss.push("ID");
                    break;
                case 25:
                    if (on)
                        output << "<F> -> <NUM>" << endl;
                    storeTree("F", vector<string> { "NUM" });
                    cout << setw(20) << "| pop(" << t << "), push(NUM)";
                    ss.push("NUM");
                    break;
                case 26:
                    if (on)
                        output << "<ID> -> " << lexeme << endl;
                    storeTree("ID", vector<string> { lexeme });
                    cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                    ss.push(lexeme);
                    break;
                case 27:
                    if (on)
                        output << "<NUM> -> " << lexeme << endl; 
                    storeTree("NUM", vector<string> { lexeme });
                    cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                    ss.push(lexeme);
                    break;
                case 28:
                    if (on)
                        output << "<NUM> -> " << lexeme << endl; 
                    storeTree("NUM", vector<string> { lexeme });
                    cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                    ss.push(lexeme);
                    break;
                case 29:
                    if (on)
                        output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                    storeTree("IF", vector<string> { "if", "C", "then", "S", "else", "S", "endif" });
                    cout << setw(20) << "| pop(" << t << "), push(endif S else S then C if)";
                    ss.push("endif");
                    ss.push("S");
                    ss.push("else");
                    ss.push("S");
                    ss.push("then");
                    ss.push("C");
                    ss.push("if");
                    break;
                case 30:
                    if (on)
                        output << "<WHILE> -> while <C> do <S> whileend" << endl;
                    storeTree("WHILE", vector<string> { "while", "C", "do", "S", "whileend" });
                    cout << setw(20) << "| pop(" << t << "), push(whileend S do C while)";
                    ss.push("whileend");
                    ss.push("S");
                    ss.push("do");
                    ss.push("C");
                    ss.push("while");
                    break;
                case 31:
                    if (on)
                        output << "<BEGIN> -> begin <S> <MS> end" << endl;
                    storeTree("BEGIN", vector<string> { "begin", "S", "MS", "end" });
                    cout << setw(20) << "| pop(" << t << "), push(end MS S begin)";
                    ss.push("end");
                    ss.push("MS");
                    ss.push("S");
                    ss.push("begin");
                    break;
                case 32:
                    if (on)
                        output << "<C> -> <E> <B>" << endl;
                    storeTree("C", vector<string> { "E", "B" });
                    cout << setw(20) << "| pop(" << t << "), push(B E)";
                    ss.push("B");
                    ss.push("E");
                    break;
                case 33:
                    if (on)
                        output << "<B> -> <RELOP> <E>" << endl;
                    storeTree("B", vector<string> { "RELOP", "E" });
                    cout << setw(20) << "| pop(" << t << "), push(E RELOP)";
                    ss.push("E");
                    ss.push("RELOP");
                    break;
                case 34:
                    if (on)
                        output << "<B> -> <empty>" << endl;
                    storeTree("B", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 35:
                    if (on)
                        output << "<MS> -> ; <S> <MS>" << endl;
                    storeTree("MS", vector<string> { ";", "S", "MS" });
                    cout << setw(20) << "| pop(" << t << "), push(MS S ;)";
                    ss.push("MS");
                    ss.push("S");
                    ss.push(";");
                    break;
                case 36:
                    if (on)
                        output << "<MS> -> <empty>" << endl;
                    storeTree("MS", vector<string> { "empty" });
                    cout << setw(20) << "| pop(" << t << "), push(empty)";
                    break;
                case 37:
                    if (on)
                        output << "<RELOP> -> <" << endl;
                    storeTree("RELOP", vector<string> { "<" });
                    cout << setw(20) << "| pop(" << t << "), push(<)";
                    ss.push("<");
                    break;
                case 38:
                    if (on)
                        output << "<RELOP> -> <=" << endl;
                    storeTree("RELOP", vector<string> { "<=" });
                    cout << setw(20) << "| pop(" << t << "), push(<=)";
                    ss.push("<=");
                    break;
                case 39:
                    if (on)
                        output << "<RELOP> -> ==" << endl;
                    storeTree("RELOP", vector<string> { "==" });
                    cout << setw(20) << "| pop(" << t << "), push(==)\n";
                    ss.push("==");
                    break;
                case 40:
                    if (on)
                        output << "<RELOP> -> <>" << endl;
                    storeTree("RELOP", vector<string> { "<>" });
                    cout << setw(20) << "| pop(" << t << "), push(<>)\n";
                    ss.push("<>");
                    break;
                case 41:
                    if (on)
                        output << "<RELOP> -> >=" << endl;
                    storeTree("RELOP", vector<string> { ">=" });
                    cout << setw(20) << "| pop(" << t << "), push(>=)\n";
                    ss.push(">=");
                    break;
                case 42:
                    if (on)
                        output << "<RELOP> -> >" << endl;
                    storeTree("RELOP", vector<string> { ">" });
                    cout << setw(20) << "| pop(" << t << "), push(>)\n";
                    ss.push(">");
                    break;
                default:
                    output << "\n***NOT ACCEPTED***\n";
                    output.close();
                    return false;
            }
        }
        cout << setw(20) << "| Rule: #" << to_string(r) << endl;
    }
    output << "\n***ACCEPTED***\n\n+--FINISH--+\n";
    output.close();
    return true;
}