#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <queue>
#include <stack>
#include "../Tables/tree.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef vector<vector<int>> production;
typedef queue<Token> symbol;
typedef Tree<string> parse;

class STACK {
private:
    ofstream output;
    stack<string> ss;

    string t; // current top of stack
    size_t r; // production rule index

    symbol table;
    parse tree;
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
    STACK(){
        r = 0;
        ss.push("%");
        ss.push("S");
    }

    void debug(stack<string>, symbol);
    void copy(symbol&);
    void parser(symbol);
    bool IS_TERMINAL(string);
    int TERMINAL(string);
    int NONTERMINAL(string);
};

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

void STACK::parser(symbol w){ 
    system("cls");
    output.open("out.txt");
    copy(w);
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
                exit(0);
            }
        }
        else {
            vector<string> rhs;
            ss.pop();
            r = rules[NONTERMINAL(t)][TERMINAL(lexeme)];
            if (r != 0){
                switch (r){
                    case 1:
                        output << "<S> -> <A>" << endl;
                        rhs.push_back("A");
                        tree.insert("S", rhs);
                        cout << setw(20) << " | pop(" << t << "), push(A)";
                        ss.push("A");
                        break;
                    case 2:
                        output << "<S> -> <D>" << endl;
                        rhs.push_back("D");
                        tree.insert("S", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(D)";
                        ss.push("D");
                        break;
                    case 3:
                        output << "<S> -> <E>" << endl;
                        rhs.push_back("E");
                        tree.insert("S", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(E)";
                        ss.push("E");
                        break;
                    case 4:
                        output << "<S> -> <IF>" << endl;
                        rhs.push_back("IF");
                        tree.insert("S", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(IF)";
                        ss.push("IF");
                        break;
                    case 5:
                        output << "<S> -> <WHILE>" << endl;
                        rhs.push_back("WHILE");
                        tree.insert("S", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(WHILE)";
                        ss.push("WHILE");
                        break;
                    case 6:
                        output << "<S> -> <BEGIN>" << endl;
                        rhs.push_back("BEGIN");
                        tree.insert("S", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(BEGIN)";
                        ss.push("BEGIN");
                        break;
                    case 7: 
                        output << "<A> -> <ID> = <E>" << endl;
                        rhs.push_back("ID");
                        rhs.push_back("=");
                        rhs.push_back("E");
                        tree.insert("A", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(E = ID)";
                        ss.push("E");
                        ss.push("=");
                        ss.push("ID");
                        break;
                    case 8:
                        output << "<D> -> <TY> <ID> <MORE_IDS> ;" << endl;
                        rhs.push_back("TY");
                        rhs.push_back("ID");
                        rhs.push_back("MID");
                        rhs.push_back(";");
                        tree.insert("D", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(; MID ID TY)";
                        ss.push(";");
                        ss.push("MID");
                        ss.push("ID");
                        ss.push("TY");
                        break;
                    case 9:
                        output << "<D> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("D", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 10:
                        output << "<TY> -> bool" << endl;
                        rhs.push_back("bool");
                        tree.insert("TY", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(bool)";
                        ss.push("bool");
                        break;
                    case 11: 
                        output << "<TY> -> int" << endl;
                        rhs.push_back("int");
                        tree.insert("TY", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(int)";
                        ss.push("int");
                        break;
                    case 12:
                        output << "<TY> -> float" << endl;
                        rhs.push_back("float");
                        tree.insert("TY", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(float)";
                        ss.push("float");
                        break;
                    case 13: 
                        output << "<MORE_IDS> -> , <ID> <MORE_IDS>\n";
                        rhs.push_back(",");
                        rhs.push_back("ID");
                        rhs.push_back("MID");
                        tree.insert("MID", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(MID ID ,)";
                        ss.push("MID");
                        ss.push("ID");
                        ss.push(",");
                        break;
                    case 14:
                        output << "<MORE_IDS> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("MID", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 15:
                        output << "<E> -> <T> <Q>" << endl;
                        rhs.push_back("T");
                        rhs.push_back("Q");
                        tree.insert("E", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(Q T)";
                        ss.push("Q");
                        ss.push("T");
                        break;
                    case 16:
                        output << "<T> -> <F> <R>" << endl;
                        rhs.push_back("F");
                        rhs.push_back("R");
                        tree.insert("T", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(R F)";
                        ss.push("R");
                        ss.push("F");
                        break;
                    case 17:
                        output << "<Q> -> + <T> <Q>" << endl;
                        rhs.push_back("+");
                        rhs.push_back("T");
                        rhs.push_back("Q");
                        tree.insert("Q", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(Q T +)";
                        ss.push("Q");
                        ss.push("T");
                        ss.push("+");
                        break;
                    case 18:
                        output << "<Q> -> - <T> <Q>" << endl;
                        rhs.push_back("-");
                        rhs.push_back("T");
                        rhs.push_back("Q");
                        tree.insert("Q", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(Q T -)";
                        ss.push("Q");
                        ss.push("T");
                        ss.push("-");
                        break;
                    case 19:
                        output << "<Q> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("Q", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 20:
                        output << "<R> -> * <F> <R>" << endl;
                        rhs.push_back("*");
                        rhs.push_back("F");
                        rhs.push_back("R");
                        tree.insert("R", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(R F *)";
                        ss.push("R");
                        ss.push("F");
                        ss.push("*");
                        break;
                    case 21: 
                        output << "<R> -> / <F> <R>" << endl;
                        rhs.push_back("/");
                        rhs.push_back("F");
                        rhs.push_back("R");
                        tree.insert("R", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(R F /)";
                        ss.push("R");
                        ss.push("F");
                        ss.push("/");
                        break;
                    case 22:
                        output << "<R> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("R", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 23: 
                        output << "<F> -> ( <E> )" << endl;
                        rhs.push_back("(");
                        rhs.push_back("E");
                        rhs.push_back(")");
                        tree.insert("F", rhs);
                        cout << setw(20) << "| pop(" << t << "), push( )E( )";
                        ss.push(")");
                        ss.push("E");
                        ss.push("(");
                        break;
                    case 24:
                        output << "<F> -> <ID>" << endl;
                        rhs.push_back("ID");
                        tree.insert("F", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(ID)";
                        ss.push("ID");
                        break;
                    case 25:
                        output << "<F> -> <NUM>" << endl;
                        rhs.push_back("NUM");
                        tree.insert("F", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(NUM)";
                        ss.push("NUM");
                        break;
                    case 26:
                        output << "<ID> -> " << lexeme << endl;
                        rhs.push_back(lexeme);
                        tree.insert("ID", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                        ss.push(lexeme);
                        break;
                    case 27:
                        output << "<NUM> -> " << lexeme << endl; 
                        rhs.push_back(lexeme);
                        tree.insert("NUM", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                        ss.push(lexeme);
                        break;
                    case 28: 
                        output << "<NUM> -> " << lexeme << endl; 
                        rhs.push_back(lexeme);
                        tree.insert("NUM", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(" << lexeme << ")";
                        ss.push(lexeme);
                        break;
                    case 29: 
                        output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                        rhs.push_back("if");
                        rhs.push_back("C");
                        rhs.push_back("then");
                        rhs.push_back("S");
                        rhs.push_back("else");
                        rhs.push_back("S");
                        rhs.push_back("endif");
                        tree.insert("IF", rhs);
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
                        output << "<WHILE> -> while <C> do <S> whileend" << endl;
                        rhs.push_back("while");
                        rhs.push_back("C");
                        rhs.push_back("do");
                        rhs.push_back("S");
                        rhs.push_back("whilend");
                        tree.insert("WHILE", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(whileend S do C while)";
                        ss.push("whileend");
                        ss.push("S");
                        ss.push("do");
                        ss.push("C");
                        ss.push("while");
                        break;
                    case 31:
                        output << "<BEGIN> -> begin <S> <MS> end" << endl;
                        rhs.push_back("begin");
                        rhs.push_back("S");
                        rhs.push_back("MS");
                        rhs.push_back("end");
                        tree.insert("BEGIN", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(end MS S begin)";
                        ss.push("end");
                        ss.push("MS");
                        ss.push("S");
                        ss.push("begin");
                        break;
                    case 32:
                        output << "<C> -> <E> <B>" << endl;
                        rhs.push_back("E");
                        rhs.push_back("B");
                        tree.insert("C", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(B E)";
                        ss.push("B");
                        ss.push("E");
                        break;
                    case 33: 
                        output << "<B> -> <RELOP> <E>" << endl;
                        rhs.push_back("RELOP");
                        rhs.push_back("E");
                        tree.insert("B", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(E RELOP)";
                        ss.push("E");
                        ss.push("RELOP");
                        break;
                    case 34: 
                        output << "<B> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("B", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 35: 
                        output << "<MS> -> ; <S> <MS>" << endl;
                        rhs.push_back(";");
                        rhs.push_back("S");
                        rhs.push_back("MS");
                        tree.insert("MS", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(MS S ;)";
                        ss.push("MS");
                        ss.push("S");
                        ss.push(";");
                        break;
                    case 36: 
                        output << "<MS> -> <empty>" << endl;
                        rhs.push_back("empty");
                        tree.insert("MS", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(empty)";
                        break;
                    case 37: 
                        output << "<RELOP> -> <" << endl;
                        rhs.push_back("<");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(<)";
                        ss.push("<");
                        break;
                    case 38: 
                        output << "<RELOP> -> <=" << endl;
                        rhs.push_back("<=");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(<=)";
                        ss.push("<=");
                        break;
                    case 39: 
                        output << "<RELOP> -> ==" << endl;
                        rhs.push_back("==");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(==)\n";
                        ss.push("==");
                        break;
                    case 40: 
                        output << "<RELOP> -> <>" << endl;
                        rhs.push_back("<>");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(<>)\n";
                        ss.push("<>");
                        break;
                    case 41: 
                        output << "<RELOP> -> >=" << endl;
                        rhs.push_back(">=");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(>=)\n";
                        ss.push(">=");
                        break;
                    case 42: 
                        output << "<RELOP> -> >" << endl;
                        rhs.push_back(">");
                        tree.insert("RELOP", rhs);
                        cout << setw(20) << "| pop(" << t << "), push(>)\n";
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
        cout << setw(20) << "| Rule: #" << to_string(r) << endl;
    }
    output << "\n***ACCEPTED***\n";
    tree.print();
    cout << "\033[32m" << "\nPASSED: " << "\033[0m" << "view in out.txt\n";
    output.close();
}
