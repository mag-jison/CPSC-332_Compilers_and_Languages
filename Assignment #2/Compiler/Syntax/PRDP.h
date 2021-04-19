#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "../Lexer/lexer.h"
#include "../Tables/error_table.h"
#include "../Tables/tree.h"

#define token table.front().tok // macros for ease of future debugging
#define lexeme table.front().lex
#define ln table.front().ln
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef queue<Token> symbol; // for ease of future debugging
typedef Tree<string> parse;

enum NONTERMINAL { STATE, DECLARE, MORE_ID, ME, TYPE, ASSIGN, EXP, EXP_PRIME, QE, TERM, FACTOR, FACTOR_PRIME, RE, IDENTIFIER, NUMBER, IS, WS, BS, BE, MRS, MSE, COND, COND_PRIME, RELA };
enum TERMINAL { SEMI_COLON, COMMA, LPAREN, RPAREN, EQUAL, THEN, ELSE, ENDIF, DO, WHILEEND, BEGIN, END, id };

class PRDP {
private:
    symbol table;
    parse tree;
    ErrorTable e;     // creates a ErrorTable object
    ofstream output;  // output file generator
    string g;         // stores all lexemes of the accepted sentences
    bool on;          // allows user to control output
    pair<size_t, string> backup;
    vector<string> rhs;

public:
    void copy(symbol&);
    string next_lexeme(symbol);  // returns the next lexeme without disturbing the queue data structure
    void lexer();                // adds lexemes and gets next token and lexeme
    void print(NONTERMINAL);     // prints the tokens and lexemes before every new production
    void print(TERMINAL);

    void parser(symbol, bool);      // recursive decent parser (top-down parser)
    bool FIRST(NONTERMINAL);
    bool FOLLOW(NONTERMINAL);
    bool IF();       // if statement production
    bool WHILE();    // while statement production
    bool BEGIN();    // begin statement production
    bool MS();       // more_statements production
    bool MID();      // more_identifiers production
    bool TY();       // type production
    bool ID();       // identifer production
    bool NUM();      // number production
    bool RELOP();    // relational operator production
    bool A();        // assignment production
    bool B();        // conditional prime production
    bool C();        // conditional production
    bool D();        // declarative production
    bool E();        // expression production
    bool F();        // factor production
    bool Q();        // expression prime production
    bool R();        // factor prime production
    bool S();        // statement production
    bool T();        // term production
};

bool PRDP::NUM() { // determines if a token is a number or not
    return (token == "INTEGER" || token == "REAL") ? true : false;
}

bool PRDP::ID() { // determines if a token is an identifier or not
    return (token == "IDENTIFIER") ? true : false;
}

bool PRDP::TY() { // determines if a token is a type or not
    return (lexeme == "bool" || lexeme == "int" || lexeme == "float") ? true : false;
}

bool PRDP::RELOP() { // determines if a token is a relational operator or not
    return (lexeme == "<" || lexeme == "<=" || lexeme == "==" || lexeme == "<>" || lexeme == ">=" || lexeme == ">") ? true : false;
}

bool PRDP::FIRST(NONTERMINAL n) {
    if (table.empty())
        return false;
    switch (n){
        case STATE:
            return (ID() || TY() || lexeme == "(" || NUM() || lexeme == "if" || lexeme == "while" || lexeme == "begin") ? true : false;
            break;
        case DECLARE:
            return (TY()) ? true : false;
            break;
        case MORE_ID:
            return (lexeme == ",") ? true : false;
            break;
        case TYPE:
            return (TY()) ? true : false;
            break;
        case ASSIGN:
            return (ID()) ? true : false;
            break;
        case EXP:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
            break;
        case EXP_PRIME:
            return (lexeme == "+" || lexeme == "-") ? true : false;
            break;
        case TERM:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
            break;
        case FACTOR:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
            break;
        case FACTOR_PRIME:
            return (lexeme == "*" || lexeme == "/") ? true : false;
            break;
        case IDENTIFIER:
            return (ID()) ? true : false;
            break;
        case NUMBER:
            return (NUM()) ? true : false;
            break;
        case IS:
            return (lexeme == "if") ? true : false;
            break;
        case WS:
            return (lexeme == "while") ? true : false;
            break;
        case BS:
            return (lexeme == "begin") ? true : false;
            break;
        case MRS:
            return (lexeme == ";") ? true : false;
            break;
        case COND:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
            break;
        case COND_PRIME:
            return (RELOP()) ? true : false;
            break;
        case RELA:
            return (RELOP()) ? true : false;
            break;
        default:
            return false;
            break;
    }
}

bool PRDP::FOLLOW(NONTERMINAL n){
    if (table.empty())
        return false;
    switch (n){
        case STATE:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case DECLARE:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case MORE_ID:
            return (lexeme == ";") ? true : false;
            break;
        case TYPE:
            return (ID()) ? true : false;
            break;
        case ASSIGN:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case EXP:
            return (lexeme == "%" || lexeme == "else" || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
            break;
        case EXP_PRIME:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
            break;
        case TERM:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-") ? true : false;
            break;
        case FACTOR:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/") ? true : false;
            break;
        case FACTOR_PRIME:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-") ? true : false;
            break;
        case IDENTIFIER:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "=" || lexeme == ",") ? true : false;
            break;
        case NUMBER:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/") ? true : false;
            break;
        case IS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case WS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case BS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
            break;
        case MRS:
            return (lexeme == "end") ? true : false;
            break;
        case COND:
            return (lexeme == "then" || lexeme == "do") ? true : false;
            break;
        case COND_PRIME:
            return (lexeme == "then" || lexeme == "do") ? true : false;
            break;
        case RELA:
             return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
             break;
        default:
            return false;
            break;
    }
}

void PRDP::print(TERMINAL t) {
    rhs.clear();
    if (on) {
        switch (t) {
            case LPAREN:
                rhs.push_back("(");
                tree.insert("F", rhs);
                output << "<F> -> ( <E>" << endl;
                break;
            case RPAREN:
                rhs.push_back(")");
                tree.insert("F", rhs);
                output << write << "<F> -> ( <E> )" << endl;
                break;
            case SEMI_COLON:
            case COMMA:
            case EQUAL:
            case THEN:
            case ELSE:
            case ENDIF:
            case DO:
            case WHILEEND:
            case END:
            case id:
            default:
                output << write; 
                break;
        }
    }
}

void PRDP::print(NONTERMINAL n){
    rhs.clear();
    if (on){
        switch (n){
            case STATE:
                rhs.push_back("A");
                rhs.push_back("D");
                rhs.push_back("E");
                rhs.push_back("IF");
                rhs.push_back("WHILE");
                rhs.push_back("BEGIN");
                tree.insert("S", rhs);
                output << write << "<S> -> <A> | <D> | <E> | <IF> | <WHILE> | <BEGIN>" << endl;
                break;
            case IS:
                rhs.push_back("if");
                rhs.push_back("C");
                rhs.push_back("then");
                rhs.push_back("S");
                rhs.push_back("else");
                rhs.push_back("S");
                rhs.push_back("endif");
                tree.insert("IF", rhs);
                output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                break;
            case COND:
                rhs.push_back("E");
                rhs.push_back("B");
                tree.insert("C", rhs);
                output << "<C> -> <E> <B>\n" << write;
                break;
            case COND_PRIME:
                rhs.push_back("RELOP");
                rhs.push_back("E");
                tree.insert("B", rhs);
                output << "<B> -> <RELOP> <E> | <empty>" << endl ;
                break;
            case RELA:
                rhs.push_back(lexeme);
                tree.insert("RELOP", rhs);
                output << write << "<RELOP> -> < | <= | == | <> | >= | >" << endl;
                break;
            case WS:
                rhs.push_back("while");
                rhs.push_back("C");
                rhs.push_back("do");
                rhs.push_back("S");
                rhs.push_back("whilend");
                tree.insert("WHILE", rhs);
                output << "<WHILE> -> while <C> do <S> whileend" << endl;
                break;
            case BS:
                rhs.push_back("begin");
                rhs.push_back("S");
                rhs.push_back("MS");
                rhs.push_back("end");
                tree.insert("BEGIN", rhs);
                output << "<BEGIN> -> begin <S> <MS> end" << endl;
                break;
            case MRS:
                rhs.push_back(";");
                rhs.push_back("S");
                rhs.push_back("MS");
                tree.insert("MS", rhs);
                output << write << "<MS> -> ; <S> <MS> | <empty>" << endl;
                break;
            case EXP:
                rhs.push_back("T");
                rhs.push_back("Q");
                tree.insert("E", rhs);
                output << "<E> -> <T> <Q>" << endl;
                break;
            case EXP_PRIME:
                rhs.push_back(lexeme);
                rhs.push_back("T");
                rhs.push_back("Q");
                tree.insert("Q", rhs);
                output << write << "<Q> -> + <T> <Q> | - <T> <Q> | <empty>" << endl;
                break;
            case TERM:
                rhs.push_back("F");
                rhs.push_back("R");
                tree.insert("T", rhs);
                output << "<T> -> <F> <R>" << endl;
                break;
            case FACTOR_PRIME:
                rhs.push_back(lexeme);
                rhs.push_back("F");
                rhs.push_back("R");
                tree.insert("R", rhs);
                output << write << "<R> -> * <F> <R> | / <F> <R> | <empty>" << endl;
                break;
            case FACTOR:
                rhs.push_back(lexeme);
                tree.insert("F", rhs);
                output << "<F> -> <ID> | <NUM> | ( <E> )" << endl;
                break;
            case NUMBER:
                rhs.push_back(lexeme);
                tree.insert("NUM", rhs);
                output << "<NUM> -> " << lexeme << endl;
                break;
            case DECLARE:
                rhs.push_back("TY");
                rhs.push_back("ID");
                rhs.push_back("MID");
                rhs.push_back(";");
                tree.insert("D", rhs);
                output << "<D> -> <TY> <ID> <MORE_IDS> ; | <empty>" << endl;
                break;
            case TYPE:
                output << "<TY> -> bool | int | float" << endl;
                rhs.push_back(lexeme);
                tree.insert("TY", rhs);
                break;
            case IDENTIFIER:
                rhs.push_back(lexeme);
                tree.insert("ID", rhs);
                output << "<ID> -> " << lexeme << endl;
                break;
            case MORE_ID:
                rhs.push_back(",");
                rhs.push_back("ID");
                rhs.push_back("MID");
                tree.insert("MID", rhs);
                output << write << "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>" << endl;
                break;
            case ASSIGN:
                rhs.push_back("ID");
                rhs.push_back("=");
                rhs.push_back("E");
                rhs.push_back(";");
                tree.insert("A", rhs);
                output << "<A> -> <ID> = <E> ;" << endl;
                break;
            case MSE:
                rhs.push_back("empty");
                tree.insert("MS", rhs);
                output << "<MS> -> <empty>" << endl;
                break;
            case ME:
                rhs.push_back("empty");
                tree.insert("MID", rhs);
                output << "<MORE_IDS> -> <empty>" << endl;
                break;
            case QE:
                rhs.push_back("empty");
                tree.insert("Q", rhs);
                output << "<Q> -> <empty>" << endl;
                break;
            case RE:
                rhs.push_back("empty");
                tree.insert("R", rhs);
                output << "<R> -> <empty>" << endl;
                break;
            case BE:
                rhs.push_back("empty");
                tree.insert("B", rhs);
                output << "<B> -> <empty>" << endl;
                break;
            default:
                break;
        }
    }
}

bool PRDP::S() { // starting symbol
    print(STATE);
    return (IF() || WHILE() || BEGIN() || A() || D() || E()) ? true : false; // returns true only if grammar is syntactically correct
}

bool PRDP::IF() {
    if (FIRST(IS)) {
        print(IS);
        lexer();
        if (FIRST(COND)) {
            C();                    // jumps to conditional
            if (lexeme == "then") { // checks if current lexeme matches
                print(THEN);        // prints current token and lexeme 
                lexer();            // adds lexeme then and gets new token and lexeme
                if (FIRST(STATE)) { // jumps to statement
                    S();
                    if (lexeme == "else") { // checks if current lexeme matches
                        print(ELSE); 
                        lexer();
                        if (FIRST(STATE)) {
                            S();
                            if (lexeme == "endif") {
                                print(ENDIF); 
                                lexer();
                                return true;
                            }
                            else {
                                e.handler(backup, "endif"); // handles errors if structure does not match
                                return false;
                            }
                        }
                        else {
                            e.handler(backup, "<S>"); // handles errors if structure does not match
                            return false;
                        }
                    }
                    else {
                        e.handler(backup, "else"); // handles errors if structure does not match
                        return false;
                    }
                }
                else {
                    e.handler(backup, "<S>"); // handles errors if structure does not match
                    return false;
                }
            }
            else {
                e.handler(backup, "then"); // handles errors if structure does not match
                return false;
            }
        }
        else {
            e.handler(backup, "<C>"); // handles errors if structure does not match
            return false;
        }
    }
    else
        return FOLLOW(IS) ? true : false;
}

bool PRDP::C() {
    print(COND);
    if (FIRST(EXP)){
        E();
        if (FIRST(COND_PRIME)){
            B();
            return true;
        }
        else {
            print(BE);
            return FOLLOW(COND_PRIME) ? true : false;
        }
    }
    else {
        e.handler(backup, "<E>");
        return false;
    }
}

bool PRDP::B(){
    print(COND_PRIME);
    if (FIRST(RELA)) { // jumps to relational operator production
        RELOP();
        print(RELA);
        lexer();
        if (FIRST(EXP)) { // jumps to expression production
            print(id);
            E();
            return true;
        }
        else {
            e.handler(backup, "<E>"); // error handles for invalid expression
            return false;
        }
    }
    else {
        e.handler(backup, "<RELOP>");
        return false;
    }
}

bool PRDP::WHILE() {
    if (FIRST(WS)) {
        print(WS);
        lexer();
        if (FIRST(COND)) {
            C();
            if (lexeme == "do") {
                print(DO);
                lexer(); // adds lexeme, 'do', to the list and gets new token and lexeme
                if (FIRST(STATE)) {
                    S();
                    if (lexeme == "whileend") { // checks to see if current lexeme matches
                        print(WHILEEND);
                        lexer();
                        return true;
                    }
                    else {
                        e.handler(backup, "whileend"); // error handling if lexeme is not 'whileend'
                        return false;
                    }
                }
                else {
                    e.handler(backup, "<S>"); // error handling if non-terminal is not a statement production
                    return false;
                }
            }
            else {
                e.handler(backup, "do"); // error handling if lexeme is not 'do'
                return false;
            }
        }
        else {
            e.handler(backup, "<C>"); // error handling if non-terminal is not a conditional production
            return false;
        }
    }
    else
        return false;
}

bool PRDP::BEGIN() {
    if (FIRST(BS)) {
		print(BS);
		lexer(); // adds 'begin' lexeme to the list and gets new token and lexeme
		if (FIRST(STATE)) {
			S();
			if (FIRST(MRS))
				MS();
            else if (FOLLOW(MRS))
                print(MSE);
			else {
			    e.handler(backup, "end"); // error handling if next lexeme is not 'end'
				return false;
			}
			print(END); 
			lexer();
			return true;
		}
		else {
			e.handler(backup, "<S>"); // error handling if next non-terminal is not a statement
			return false;
		}
    }
    else
        return false;
}

bool PRDP::MS() {
    print(MRS);
    lexer();
    if (FIRST(STATE)) {
        S();
        if (FIRST(MRS)){
            MS();
            return true;
        }
        else
            return false;
    }
    else {
        e.handler(backup, "<S>");
        return false;
    }
}
bool PRDP::E() {
    if (FIRST(EXP)) {
        print(EXP);
        if (FIRST(TERM)) { // jumps to term production
            T();
            if (FIRST(EXP_PRIME)) { // jumps to expression prime production
                Q();
                return true;
            }
            else {
                print(QE);
                return FOLLOW(EXP_PRIME) ? true : false;
           }
        }
        else
            return false;
    }
    else
        return false;
}

bool PRDP::Q() {
    if (lexeme == "+" || lexeme == "-") {
        print(EXP_PRIME);
        lexer(); // adds sign, + or -, to list and gets new token and lexeme
        if (FIRST(TERM)) {
            print(id);
            T();
            Q();
            return true;
        }
        else
            return false;
    }
    return false;
}

bool PRDP::T() {
    print(TERM);
    if (FIRST(FACTOR)) { // jumps to factor production
        F();
        if (FIRST(FACTOR_PRIME)){ // jumps to factor prime production
            R();
            return true;
        }
        else {
            print(RE);
            return FOLLOW(FACTOR_PRIME) ? true : false;
        }
    }
    else
        return false;
}

bool PRDP::R() {
    if (lexeme == "*" || lexeme == "/") {
        print(FACTOR_PRIME);
        lexer();
        if (FIRST(FACTOR)) {
            print(id);
            F();
            R();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool PRDP::F() {
    print(FACTOR);
    if (lexeme == "(") { // checks if current lexeme matches
        print(LPAREN);
        lexer(); // adds lexeme, (, to list and gets new token and lexeme
        if (FIRST(EXP)) {
            print(id);
            E();
            if (lexeme == ")") {
                print(RPAREN);
                lexer();
                return true;
            }
            else {
                e.handler(backup, ")"); // error handing for missing a end parenthesis
                return false;
            }
        }
        else {
            e.handler(backup, "<E>"); // error handling for invalid token
            return false;
        }
    }
    else if (FIRST(IDENTIFIER)) { // checks if next token is of a valid identifier
        print(IDENTIFIER);
        lexer();
        return true;
    }
    else if (FIRST(NUMBER)) { // checks if next token is of a valid number
        print(NUMBER);
        lexer();
        return true;
    }
    else {
        e.handler(backup, "= | ( | <ID> | <NUM>"); // error handler if invalid expression which could
        return false;                              // start with assignment, parenthesis, identifier or number
    }
}

bool PRDP::D() {
    if (!table.empty()) {
        if (FIRST(TYPE)) {
            TY();
            print(DECLARE);
            print(TYPE);
            lexer(); // adds the type to list and gets new token and lexeme
            if (FIRST(IDENTIFIER)) {
                print(id);
                print(IDENTIFIER);
                lexer(); // adds the identifier to list and gets new token and lexeme
                if (table.empty())
                    return true;
                if (FIRST(MORE_ID)) {
                    MID(); // checks if there are multiple declarations in the grammar
                    return true;
                }
                else if (FOLLOW(MORE_ID)) { // checks if lexeme matches
                    print(ME);
                    print(SEMI_COLON);
                    lexer();
                    return true;
                }
                else {
                    print(ME);
                    return true;
                }
            }
            else {
                e.handler(backup, "<ID>"); // error handling if lexeme does not match an identifier after type
                return false;
            }
        }
        else
            return FOLLOW(DECLARE) ? true : false;
    }
    else
        return false;
}

bool PRDP::MID() {
	print(MORE_ID);
	lexer();
	if (FIRST(IDENTIFIER)) {
        print(id);
		print(IDENTIFIER);
		lexer(); // adds the identifer into list and gets new token and lexeme
        if (FIRST(MORE_ID))
    		MID(); // checks to see if any more declarations are being made on the same line
        else if (FOLLOW(MORE_ID)) {
            print(ME);
            print(COMMA);
            lexer();
            return true;
        }
        else {
            print(ME); // adds epsilon for absense or spontaneous input
            return true;
        }
	}
	else {
        e.handler(backup, "<ID>"); // error handler if missing a comma for multiple declarative grammar 
        return false;
	}
}

bool PRDP::A() {
    if (!table.empty()) {
        if (FIRST(IDENTIFIER)) {
            if (next_lexeme(table) == "=") {
                print(ASSIGN);
                print(IDENTIFIER);
                lexer();
                print(EQUAL); // prints current token and lexeme which is now an '=' sign
                lexer(); // adds identifier and gets new token and lexeme
                if (FIRST(EXP)) {
                    print(id);
                    E();
                    if (lexeme == ";") { // check if lexeme matches
                        print(SEMI_COLON); 
                        lexer();
                    }
                    return true;
                }
                else {
                    e.handler(backup, "<E>"); // handles errors if structure does not match
                    return false;
                }
            }
            else
                return false; // does not handle error because structure could be an expression rather than an assignment
        }
        else
            return false;
    }
    else
        return false; // does not handle error because structure could be an expression rather than an assignment
}

void PRDP::lexer() {
    g = g + lexeme + " "; // adds lexeme to list of other lexemes of the current sentence
    backup = { ln, lexeme };
    table.pop(); // gets next token and lexeme to continue processing
}

string PRDP::next_lexeme(symbol w) {
    w.pop(); // returns the next token without ruining the current queue data structure
    return w.front().lex;
}
 
void PRDP::copy(symbol& w) {
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

void PRDP::parser(symbol w, bool b) {
    system("cls"); // clears the screen
    copy(w);
    on = b;
    output.open("out.txt"); // opens file for input
    output << "+--START--+\n"; // start of processing

    if (table.empty()) {
        cout << "\033[91m" << "WARNING: " << "\033[0m" << "reading empty file!" << endl;
        exit(0);
    }

    while (!table.empty()) {
        g = ""; // resets the list of lexemes for new structures
        if (S())
            output << "\n***ACCEPTED***" << " => " << g << endl;
        else
            output << "\n***NOT ACCEPTED***\n"; // file prompt
    }

    output << "\n+--FINISH--+"; // finishes processing successfully
    output.close(); // closes opened file

    cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n\n"; // code is syntactically correct
    tree.print();
}
