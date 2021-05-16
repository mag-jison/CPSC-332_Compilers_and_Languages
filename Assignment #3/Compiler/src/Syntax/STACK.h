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
#define type table.front().typ
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef vector<vector<int>> production;
typedef queue<Token> symbol;

class STACK : public SymbolTable, public ErrorTable, public Tree<string> {
private:
    ofstream output;
    stack<string> ss;
    bool on;

    string t, g; // top of stack
    size_t r;    // production rule index

    symbol table;
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
    STACK(){ // initializes the stack with the starting symbol followed by a unique character
        ss.push("%");
        ss.push("S");
    }

    void sspace(int);
    Node<string>* getRoot(); // returns the root
    void getTree(); // prints out the stored tree in preOrderTraversal
    void debug(stack<string>, symbol); // prints out the traversal out to the console
    void copy(symbol&); // transfers the symbol table to the STACK class
    bool parser(symbol, bool);
    bool IS_TERMINAL(string); // returns a boolean value if input string is a terminal symbol
    int TERMINAL(string); // returns an integer value corresponding to the column of the table
    int NONTERMINAL(string); // returns an integer value corresponding to the row of the table
};

void STACK::getTree(){
    printTree();
}

Node<string>* STACK::getRoot(){
    return getRoot();
}

void STACK::sspace(int x){
    for (int i = 0; i < x-(g.length()); ++i)
        output << " ";
}

void STACK::debug(stack<string> temp, symbol table){
    g = "";
    output << "[ ";
    while (!temp.empty()){
        g = g + temp.top() + " ";
        temp.pop();
    }
    output << g << "]";
    sspace(35);
    g = "";
    while (!table.empty()){
        g += lexeme;
        table.pop();
    }
    output << g;
    sspace(45);
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

    while (!ss.empty()) {
        t = ss.top();
        debug(ss, table); // prints out the actions
        if (IS_TERMINAL(t)){
            if (t == lexeme){ // checks to see of the current input token matches with the top of the stack
                g = "pop(" + t + "), lexer()";
                output << g;
                ss.pop();
                table.pop(); // increments input pointer to continue processing
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
                        cout << "<S> -> <A>" << endl;
                    makeTree("S", vector<string> { "A" });
                    g = "pop(" + t + "), push(A)";
                    ss.push("A");
                    break;
                case 2:
                    if (on)
                        cout << "<S> -> <D>" << endl;
                    makeTree("S", vector<string> { "D" });
                    g = "pop(" + t + "), push(D)";
                    ss.push("D");
                    break;
                case 3:
                    if (on)
                        cout << "<S> -> <E>" << endl;
                    makeTree("S", vector<string> { "E" });
                    g = "pop(" + t + "), push(E)";
                    ss.push("E");
                    break;
                case 4:
                    if (on)
                        cout << "<S> -> <IF>" << endl;
                    makeTree("S", vector<string> { "IF" });
                    g = "pop(" + t + "), push(IF)";
                    ss.push("IF");
                    break;
                case 5:
                    if (on)
                        cout << "<S> -> <WHILE>" << endl;
                    makeTree("S", vector<string> { "WHILE" });
                    g = "pop(" + t + "), push(WHILE)";
                    ss.push("WHILE");
                    break;
                case 6:
                    if (on)
                        cout << "<S> -> <BEGIN>" << endl;
                    makeTree("S", vector<string> { "BEGIN" });
                    g = "pop(" + t + "), push(BEGIN)";
                    ss.push("BEGIN");
                    break;
                case 7:
                    if (on)
                        cout << "<A> -> <ID> = <E>" << endl;
                    makeTree("A", vector<string> { "ID", "=", "E" });
                    g = "pop(" + t + "), push(E = ID)";
                    ss.push("E");
                    ss.push("=");
                    ss.push("ID");
                    break;
                case 8:
                    if (on)
                        cout << "<D> -> <TY> <ID> <MORE_IDS> ;" << endl;
                    makeTree("D", vector<string> { "TY", "ID", "MID", ";" });
                    g = "pop(" + t + "), push(; MID ID TY)";
                    ss.push(";");
                    ss.push("MID");
                    ss.push("ID");
                    ss.push("TY");
                    break;
                case 9:
                    if (on)
                        cout << "<D> -> <empty>" << endl;
                    makeTree("D", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 10:
                    if (on)
                        cout << "<TY> -> bool" << endl;
                    makeTree("TY", vector<string> { "bool" });
                    g = "pop(" + t + "), push(bool)";
                    ss.push("bool");
                    break;
                case 11:
                    if (on)
                        cout << "<TY> -> int" << endl;
                    makeTree("TY", vector<string> { "int" });
                    g = "pop(" + t + "), push(int)";
                    ss.push("int");
                    break;
                case 12:
                    if (on)
                        cout << "<TY> -> float" << endl;
                    makeTree("TY", vector<string> { "float" });
                    g = "pop(" + t + "), push(float)";
                    ss.push("float");
                    break;
                case 13:
                    if (on)
                        cout << "<MORE_IDS> -> , <ID> <MORE_IDS>" << endl;
                    makeTree("MID", vector<string> { ",", "ID", "MID" });
                    g = "pop(" + t + "), push(MID ID ,)";
                    ss.push("MID");
                    ss.push("ID");
                    ss.push(",");
                    break;
                case 14:
                    if (on)
                        cout << "<MORE_IDS> -> <empty>" << endl;
                    makeTree("MID", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 15:
                    if (on)
                        cout << "<E> -> <T> <Q>" << endl;
                    makeTree("E", vector<string> { "T", "E" });
                    g = "pop(" + t + "), push(Q T)";
                    ss.push("Q");
                    ss.push("T");
                    break;
                case 16:
                    if (on)
                        cout << "<T> -> <F> <R>" << endl;
                    makeTree("T", vector<string> { "F", "R" });
                    g = "pop(" + t + "), push(R F)";
                    ss.push("R");
                    ss.push("F");
                    break;
                case 17:
                    if (on)
                        cout << "<Q> -> + <T> <Q>" << endl;
                    makeTree("Q", vector<string> { "+", "T", "Q" });
                    g = "pop(" + t + "), push(Q T +)";
                    ss.push("Q");
                    ss.push("T");
                    ss.push("+");
                    break;
                case 18:
                    if (on)
                        cout << "<Q> -> - <T> <Q>" << endl;
                    makeTree("Q", vector<string> { "-", "T", "Q" });
                    g = "pop(" + t + "), push(Q T -)";
                    ss.push("Q");
                    ss.push("T");
                    ss.push("-");
                    break;
                case 19:
                    if (on)
                        cout << "<Q> -> <empty>" << endl;
                    makeTree("Q", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 20:
                    if (on)
                        cout << "<R> -> * <F> <R>" << endl;
                    makeTree("R", vector<string> { "*", "F", "R" });
                    g = "pop(" + t + "), push(R F *)";
                    ss.push("R");
                    ss.push("F");
                    ss.push("*");
                    break;
                case 21:
                    if (on)
                        cout << "<R> -> / <F> <R>" << endl;
                    makeTree("R", vector<string> { "/", "F", "R" });
                    g = "pop(" + t + "), push(R F /)";
                    ss.push("R");
                    ss.push("F");
                    ss.push("/");
                    break;
                case 22:
                    if (on)
                        cout << "<R> -> <empty>" << endl;
                    makeTree("R", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 23:
                    if (on)
                        cout << "<F> -> ( <E> )" << endl;
                    makeTree("F", vector<string> { "(", "E", ")" });
                    g = "pop(" + t + "), push( )E( )";
                    ss.push(")");
                    ss.push("E");
                    ss.push("(");
                    break;
                case 24:
                    if (on)
                        cout << "<F> -> <ID>" << endl;
                    makeTree("F", vector<string> { "ID" });
                    g = "pop(" + t + "), push(ID)";
                    ss.push("ID");
                    break;
                case 25:
                    if (on)
                        cout << "<F> -> <NUM>" << endl;
                    makeTree("F", vector<string> { "NUM" });
                    g = "pop(" + t + "), push(NUM)";
                    ss.push("NUM");
                    break;
                case 26:
                    if (on)
                        cout << "<ID> -> " << lexeme << endl;
                    makeTree("ID", vector<string> { lexeme });
                    g = "pop(" + t + "), push(" + lexeme + ")";
                    ss.push(lexeme);
                    break;
                case 27:
                    if (on)
                        cout << "<NUM> -> " << lexeme << endl; 
                    makeTree("NUM", vector<string> { lexeme });
                    g = "pop(" + t + "), push" + lexeme + ")";
                    ss.push(lexeme);
                    break;
                case 28:
                    if (on)
                        cout << "<NUM> -> " << lexeme << endl; 
                    makeTree("NUM", vector<string> { lexeme });
                    g = "pop(" + t + "), push(" + lexeme + ")";
                    ss.push(lexeme);
                    break;
                case 29:
                    if (on)
                        cout << "<IF> -> if <C> then <S> else <S> endif" << endl;
                    makeTree("IF", vector<string> { "if", "C", "then", "S", "else", "S", "endif" });
                    g = "pop(" + t + "), push(endif S else S then C if)";
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
                        cout << "<WHILE> -> while <C> << endl do <S> whileend" << endl;
                    makeTree("WHILE", vector<string> { "while", "C", "do", "S", "whileend" });
                    g = "pop(" + t + "), push(whileend S do C while)";
                    ss.push("whileend");
                    ss.push("S");
                    ss.push("do");
                    ss.push("C");
                    ss.push("while");
                    break;
                case 31:
                    if (on)
                        cout << "<BEGIN> -> begin <S> << endl <MS> end" << endl;
                    makeTree("BEGIN", vector<string> { "begin", "S", "MS", "end" });
                    g = "pop(" + t + "), push(end MS S begin)";
                    ss.push("end");
                    ss.push("MS");
                    ss.push("S");
                    ss.push("begin");
                    break;
                case 32:
                    if (on)
                        cout << "<C> -> <E> <B>" << endl;
                    makeTree("C", vector<string> { "E", "B" });
                    g = "pop(" + t + "), push(B E)";
                    ss.push("B");
                    ss.push("E");
                    break;
                case 33:
                    if (on)
                        cout << "<B> -> <RELOP> <E>" << endl;
                    makeTree("B", vector<string> { "RELOP", "E" });
                    g = "pop(" + t + "), push(E RELOP)";
                    ss.push("E");
                    ss.push("RELOP");
                    break;
                case 34:
                    if (on)
                        cout << "<B> -> <empty>" << endl;
                    makeTree("B", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 35:
                    if (on)
                        cout << "<MS> -> ; <S> <MS>" << endl;
                    makeTree("MS", vector<string> { ";", "S", "MS" });
                    g = "pop(" + t + "), push(MS S ;)";
                    ss.push("MS");
                    ss.push("S");
                    ss.push(";");
                    break;
                case 36:
                    if (on)
                        cout << "<MS> -> <empty>" << endl;
                    makeTree("MS", vector<string> { "empty" });
                    g = "pop(" + t + "), push(empty)";
                    break;
                case 37:
                    if (on)
                        cout << "<RELOP> -> <" << endl;
                    makeTree("RELOP", vector<string> { "<" });
                    g = "pop(" + t + "), push(<)";
                    ss.push("<");
                    break;
                case 38:
                    if (on)
                        cout << "<RELOP> -> <=" << endl;
                    makeTree("RELOP", vector<string> { "<=" });
                    g = "pop(" + t + "), push(<=)";
                    ss.push("<=");
                    break;
                case 39:
                    if (on)
                        cout << "<RELOP> -> ==" << endl;
                    makeTree("RELOP", vector<string> { "==" });
                    g = "pop(" + t + "), push(==)";
                    ss.push("==");
                    break;
                case 40:
                    if (on)
                        cout << "<RELOP> -> <>" << endl;
                    makeTree("RELOP", vector<string> { "<>" });
                    g = "pop(" + t + "), push(<>)";
                    ss.push("<>");
                    break;
                case 41:
                    if (on)
                        cout << "<RELOP> -> >=" << endl;
                    makeTree("RELOP", vector<string> { ">=" });
                    g = "pop(" + t + "), push(>=)";
                    ss.push(">=");
                    break;
                case 42:
                    if (on)
                        cout << "<RELOP> -> >" << endl;
                    makeTree("RELOP", vector<string> { ">" });
                    g = "pop(" + t + "), push(>)";
                    ss.push(">");
                    break;
                default:
                    output << "\n***NOT ACCEPTED***\n";
                    output.close();
                    return false;
            }
            output << g;
        }
        sspace(45);
        output << "Rule: #" << to_string(r) << endl;
    }
    output << "\n+--FINISH--+";
    output.close();
    return true;
}