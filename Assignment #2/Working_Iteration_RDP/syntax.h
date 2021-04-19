#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "lexer.h"
#include "error_table.h"

#define token table.front().tok // macros for ease of future debugging
#define lexeme table.front().lex
#define ln table.front().ln

using namespace std;

typedef queue<Token> symbol; // for ease of future debugging

class Syntax {
private:
    symbol table;
    ErrorTable e;     // creates a ErrorTable object
    ofstream output;  // output file generator
    string g;         // stores all lexemes of the accepted sentences
    bool on = false;  // allows user to control output

public:
    void copy(symbol);
    string next_lexeme(symbol);  // returns the next lexeme without disturbing the queue data structure
    void add();                  // adds lexemes and gets next token and lexeme
    void print(int);             // prints the tokens and lexemes before every new production

    void RDP(symbol, bool); // recursive decent parser; top-down parser
    bool FOLLOW(string);
    bool IF();       // if statement production
    bool WHILE();    // while statement production
    bool BEGIN();    // begin statement production
    bool MS();       // more_statements production
    bool MID();      // more_identifiers production
    bool TYPE();     // type production
    bool ID();       // identifer production
    bool NUM();      // number production
    bool RELOP();    // relational operator production
    bool A();        // assignment production
    bool C();        // conditional production
    bool D();        // declarative production
    bool E();        // expression production
    bool F();        // factor production
    bool Q();        // expression prime production
    bool R();        // factor prime production
    bool S();        // statement production
    bool T();        // term production
};

bool Syntax::NUM() { // determines if a token is a number or not
    return (token == "INTEGER" || token == "REAL") ? true : false;
}

bool Syntax::ID() { // determines if a token is an identifier or not
    return (token == "IDENTIFIER") ? true : false;
}

bool Syntax::TYPE() { // determines if a token is a type or not
    return (lexeme == "bool" || lexeme == "int" || lexeme == "float") ? true : false;
}

bool Syntax::RELOP() { // determines if a token is a relational operator or not
    return (lexeme == "<" || lexeme == "<=" || lexeme == "==" || lexeme == "<>" || lexeme == ">=" || lexeme == ">") ? true : false;
}

bool Syntax::FOLLOW(string f) {
    if (f == "<S>")
        return (ID() || TYPE() || lexeme == "(" || NUM() || lexeme == "if" || lexeme == "while" || lexeme == "begin") ? true : false;
    else if (f == "<D>")
        return (TYPE()) ? true : false;
    else if (f == "<MID>")
        return (lexeme == ",") ? true : false;
    else if (f == "<TY>")
        return (TYPE()) ? true : false;
    else if (f == "<A>")
        return (ID()) ? true : false;
    else if (f == "<E>")
        return (lexeme == "(" || ID() || NUM()) ? true : false;
    else if (f == "<Q>")
        return (lexeme == "+" || lexeme == "-") ? true : false;
    else if (f == "<T>")
        return (lexeme == "(" || ID() || NUM()) ? true : false;
    else if (f == "<F>")
        return (lexeme == "(" || ID() || NUM()) ? true : false;
    else if (f == "<R>")
        return (lexeme == "*" || lexeme == "/") ? true : false;
    else if (f == "<ID>")
        return (ID()) ? true : false;
    else if (f == "<NUM>")
        return (NUM()) ? true : false;
    else if (f == "<IF>")
        return (lexeme == "if") ? true : false;
    else if (f == "<WHILE>")
        return (lexeme == "while") ? true : false;
    else if (f == "<BEGIN>")
        return (lexeme == "begin") ? true : false;
    else if (f == "<MS>")
        return (lexeme == ";") ? true : false;
    else if (f == "<C>")
        return (lexeme == "(" || ID() || NUM()) ? true : false;
    else if (f == "<RELOP>")
        return (RELOP()) ? true : false;
    else
        return false;
}

void Syntax::print(int i){
    if (on){
        switch (i){
            case 1:
                output << "<S> -> <A> | <D> | <E> | <IF> | <WHILE> | <BEGIN>" << endl;
                break;
            case 2:
                output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                break;
            case 3:
                output << "<C> -> <E> <RELOP> <E> | <E>" << endl;
                print(25);
                break;
            case 4:
                print(25);
                output << "<RELOP> -> < | <= |  == |  <> | >= | >" << endl;
                break;
            case 5:
                output << "<WHILE> -> while <C> do <S> whileend" << endl;
                break;
            case 6:
                output << "<BEGIN> -> begin <S> <MS> end" << endl;
                break;
            case 7:
                print(25);
                output << "<MS> -> ; <S> <MS> | <empty>" << endl;
                break;
            case 8:
                output << "<MS> -> <empty>" << endl;
                break;
            case 9:
                output << "<E> -> <T> <Q>" << endl;
                break;
            case 10:
                print(25);
                output << "<Q> -> + <T> <Q> | - <T> <Q> | <empty>" << endl;
                break;
            case 11:
                output << "<Q> -> <empty>" << endl;
                break;
            case 12:
                output << "<T> -> <F> <R>" << endl;
                break;
            case 13:
                print(25);
                output << "<R> -> * <F> <R> | / <F> <R> | <empty>" << endl;
                break;
            case 14:
                output << "<R> -> <empty>" << endl;
                break;
            case 15:
                output << "<F> -> ( <E>" << endl;
                break;
            case 16:
                print(25);
                output << "<F> -> ( <E> )" << endl;
                break;
            case 17:
                output << "<F> -> <ID> | <NUM> | ( <E> )\n<ID> -> " << lexeme << endl;
                break;
            case 18:
                output << "<F> -> <ID> | <NUM> | ( <E> )\n<NUM> -> " << lexeme << endl;
                break;
            case 19:
                output << "<D> -> <TYPE> <ID> <MORE_IDS> ; | <empty>\n<TYPE> -> bool | int | float" << endl;
                break;
            case 20:
                output << "<ID> -> " << lexeme << endl;
                break;
            case 21:
                print(25);
                output << "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>" << endl;
                break;
            case 22:
                print(25);
                output << "<ID> -> " << lexeme << endl;
                break;
            case 23:
                output << "<MORE_IDS> -> <empty>" << endl;
                break;
            case 24:
                output << "<A> -> <ID> = <E> ;\n<ID> -> " << lexeme << endl;
                break;
            case 25:
                output << "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n";
                break;
            default:
                break;
        }
    }
}


bool Syntax::S() { // starting symbol
    print(1);
    if (!table.empty())
        return (IF() || WHILE() || BEGIN() || A() || D() || E()) ? true : false; // returns true only if grammar is syntactically correct
    else
        return false;
}

bool Syntax::IF() {
    if (!table.empty()) {
        if (lexeme == "if") {
            print(2);
            add();
            if (C()) {                  // jumps to conditional
                if (lexeme == "then") { // checks if current lexeme matches
                    print(25);          // prints current token and lexeme 
                    add();              // adds lexeme then and gets new token and lexeme
                    print(25);          // prints next token and lexeme
                    if (S()) {          // jumps to statement
                        if (lexeme == "else") { // checks if current lexeme matches
                            print(25); 
                            add();
                            print(25); 
                            if (S()) {
                                if (lexeme == "endif") {
                                    print(25); 
                                    add();
                                    return true;
                                }
                                else {
                                    e.handler(ln, lexeme, "endif"); // handles errors if structure does not match
                                    table.pop();
                                    return false;
                                }
                            }
                            else {
                                e.handler(ln, lexeme, "<S>"); // handles errors if structure does not match
                                table.pop();
                                return false;
                            }
                        }
                        else {
                            e.handler(ln, lexeme, "else"); // handles errors if structure does not match
                            table.pop();
                            return false;
                        }
                    }
                    else {
                        e.handler(ln, lexeme, "<S>"); // handles errors if structure does not match
                        table.pop();
                        return false;
                    }
                }
                else {
                    e.handler(ln, lexeme, "then"); // handles errors if structure does not match
                    table.pop();
                    return false;
                }
            }
            else {
                e.handler(ln, lexeme, "<C>"); // handles errors if structure does not match
                table.pop();
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::C() {
    print(3);
    if (!table.empty()) {
        if (E()) { // jumps to expression production
            if (RELOP()) { // jumps to relational operator production
                print(4);
                add();
                print(25); 
                if (E()) // jumps to expression production
                    return true;
                else {
                    e.handler(ln, lexeme, "<E>"); // error handles for invalid expression
                    table.pop();
                    return false;
                }
            }
            else
                return true;
        }
        else {
            e.handler(ln, lexeme, "<C>"); // error handles if current non-terminal is not a conditonal production
            table.pop();
            return false;
        }
    }
    else {
        e.handler(ln, lexeme, "<C>"); // error handles if current non-terminal is not a conditonal production
        table.pop();
        return false;
    }
}

bool Syntax::WHILE() {
    if (!table.empty()) {
        if (lexeme == "while") {
            print(5);
            add();
            if (C()) {
                if (lexeme == "do") {
                    print(25); 
                    add();       // adds lexeme, 'do', to the list and gets new token and lexeme
                    print(25);   // prints current token and lexeme
                    if (S()) {
                        if (lexeme == "whileend") { // checks to see if current lexeme matches
                            print(25); 
                            add();
                            return true;
                        }
                        else {
                            e.handler(ln, lexeme, "whileend"); // error handling if lexeme is not 'whileend'
                            table.pop();
                            return false;
                        }
                    }
                    else {
                        e.handler(ln, lexeme, "<S>"); // error handling if non-terminal is not a statement production
                        table.pop();
                        return false;
                    }
                }
                else {
                    e.handler(ln, lexeme, "do"); // error handling if lexeme is not 'do'
                    table.pop();
                    return false;
                }
            }
            else {
                e.handler(ln, lexeme, "<C>"); // error handling if non-terminal is not a conditional production
                table.pop();
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::BEGIN() {
    if (!table.empty()) {
        if (lexeme == "begin") {
            print(6);
            add();       // adds 'begin' lexeme to the list and gets new token and lexeme
            print(25);   // prints current token and lexeme from queue data structure
            if (S()) {
                if (MS()) {
                    if (lexeme == "end") { // checks if current lexeme matches
                        print(25); 
                        add();
                        return true;
                    }
                    else {
                        e.handler(ln, lexeme, "end"); // error handling if next lexeme is not 'end'
                        table.pop();
                        return false;
                    }
                }
                else {
                    e.handler(ln, lexeme, "<MS>"); // error handling if next non-terminal is not more_statements
                    table.pop();
                    return false;
                }
            }
            else {
                e.handler(ln, lexeme, "<S>"); // error handling if next non-terminal is not a statement
                table.pop();
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::MS() {
    if (!table.empty()) {
        if (lexeme == ";") { // checks if current lexeme matches
            print(7);
            add();
            print(25); 
            if (S()) {
                if (MS())
                    return true;
                else
                    return false;
            }
            else {
                e.handler(ln, lexeme, "<S>");
                table.pop();
                return false;
            }
        }
        else {
            print(8); // epsilon allows for absense or spontaneuous inputs
            return true;
        }
    }
    else {
        print(8);
        return true;
    }
}

bool Syntax::E() {
    print(9);
    if (!table.empty()) {
        if (T()) {     // jumps to term production
            if (Q())   // jumps to expression prime production
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::Q() {
    if (!table.empty()) {
        if (lexeme == "+" || lexeme == "-") {
            print(10);
            add();       // adds sign, + or -, to list and gets new token and lexeme
            print(25);   // prints current token and lexeme for queue data structure
            if (T()) {
                if (Q())
                    return true;
                else
                    return false;
            }
            else
                return false;
        }
        else {
            print(11); // epsilon allows for absense or spontaneuous inputs
            return true;
        }
    }
    else {
        print(11);
        return true;
    }
}

bool Syntax::T() {
    print(12); 
    if (!table.empty()) {
        if (F()) {   // jumps to factor production
            if (R()) // jumps to factor prime production
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::R() {
    if (!table.empty()) {
        if (lexeme == "*" || lexeme == "/") {
            print(13);
            add();
            print(25); 
            if (F()) {
                if (R())
                    return true;
                else
                    return false; // no error handling because it will be taken care of as an invalid expression later in the code
            }
            else
                return false;
        }
        else {
            print(14); // epsilon allows for absense or spontaneuous inputs
            return true;
        }
    }
    else {
        print(14);
        return true;
    }
}

bool Syntax::F() {
    if (!table.empty()) {
        if (lexeme == "(") { // checks if current lexeme matches
            print(15);
            add();     // adds lexeme, (, to list and gets new token and lexeme
            print(25); // prints current token and lexeme which is most likely an expression
            if (E()) {
                if (lexeme == ")") {
                    print(16);
                    add();
                    return true;
                }
                else {
                    e.handler(ln, lexeme, ")"); // error handing for missing a end parenthesis
                    table.pop();
                    return false;
                }
            }
            else {
                e.handler(ln, lexeme, "<E>"); // error handling for invalid token
                table.pop();
                return false;
            }
        }
        else if (ID()) { // checks if next token is of a valid identifier
            print(17);
            add();
            return true;
        }
        else if (NUM()) { // checks if next token is of a valid number
            print(18);
            add();
            return true;
        }
        else {
            e.handler(ln, lexeme, "= | ( | <ID> | <NUM>"); // error handler if invalid expression which could
            table.pop();
            return false;                                  // start with assignment, parenthesis, identifier or number
        }
    }
    else {
        e.handler(ln, lexeme, "= | ( | <ID> | <NUM>");
        table.pop();
        return false;
    }
}

bool Syntax::D() {
    if (!table.empty()) {
        if (TYPE()) {
            print(19);
            add();     // adds the type to list and gets new token and lexeme
            print(25); // prints current token and lexeme
            if (ID()) {
                print(20);
                add(); // adds the identifier to list and gets new token and lexeme
                MID(); // checks if there are multiple declarations in the grammar
                if (lexeme == ";") { // checks if lexeme matches
                    print(25); 
                    add();
                }
                return true;
            }
            else {
                e.handler(ln, lexeme, "<ID>"); // error handling if lexeme does not match an identifier after type
                table.pop();
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

bool Syntax::MID() {
    if (!table.empty()) {
        if (lexeme == ",") {
            g = g + lexeme + " ";
            print(21);
            table.pop();
            if (ID()) {
                print(22);
                add(); // adds the identifer into list and gets new token and lexeme
                MID(); // checks to see if any more declarations are being made on the same line
            }
            else {
                e.handler(ln, lexeme, "<ID>"); // error handler if missing a comma for multiple declarative grammar 
                table.pop();
                return false;
            }
        }
        else {
            print(23); // adds epsilon for absense or spontaneous input
            return true;
        }
    }
    else {
        print(23);
        return true;
    }
}

bool Syntax::A() {
    if (!table.empty()) {
        if (ID()) {
            if (next_lexeme(table) == "=") {
                print(24);
                add();       // adds identifier and gets new token and lexeme
                print(25);   // prints current token and lexeme which is now an '=' sign
                add();       // adds equal sign and get new token and lexeme
                print(25);   // prints current token and lexeme which most likely an expression; else, will prompt error
                if (E()) {
                    if (lexeme == ";") { // check if lexeme matches
                        print(25); 
                        add();
                    }
                    return true;
                }
                else {
                    e.handler(ln, lexeme, "<E>"); // handles errors if structure does not match
                    table.pop();
                    return false;
                }
            }
            else
                return false; // does not handle error because structure could be an expression rather than an assignment
        }
        return false;
    }
    else
        return false; // does not handle error because structure could be an expression rather than an assignment
}

void Syntax::add() {
    g = g + lexeme + " "; // adds lexeme to list of other lexemes of the current sentence
    table.pop();          // gets next token and lexeme to contitable.pop();nue processing
}

string Syntax::next_lexeme(symbol w) {
    w.pop(); // returns the next token witable.pop();thout ruining the current queue data structure
    return w.front().lex;
}

void Syntax::copy(symbol w) {
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

void Syntax::RDP(symbol w, bool b) {
    system("cls"); // clears the screen
    output.open("out.txt"); // opens file for input
    copy(w);
    on = b;

    output << "+--START--+" << endl; // start of processing
    while (!table.empty()) {
        g = ""; // resets the list of lexemes for new structures
       print(25); 
        if (S())
            output << "\n***ACCEPTED***" << " => " << g << endl;
        else
            output << "\n***NOT ACCEPTED***" << endl; // file prompt
    }

    if (e.empty())
        cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n"; // code is syntactically correct
    else
        e.display(); // error handling for invalid grammar

    output << "\n+--FINISH--+"; // finishes processing successfully
    output.close(); // closes opened file
}
