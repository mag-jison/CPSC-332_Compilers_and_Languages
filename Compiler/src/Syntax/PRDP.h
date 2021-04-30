#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>
#include "../Tables/symbol.h"
#include "../Tables/error.h"
#include "../Tables/tree.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define types table.front().ttype
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef queue<Token> symbol;

enum NONTERMINAL { STATE, DECLARE, MORE_ID, ME, TYPE, ASSIGN, EXP, EXP_PRIME, QE, TERM, FACTOR, FACTOR_PRIME, RE, IDENTIFIER, NUMBER, IS, WS, BS, BE, MRS, MSE, COND, COND_PRIME, RELA };
enum TERMINAL { SEMI_COLON, COMMA, LPAREN, RPAREN, EQUAL, THEN, ELSE, ENDIF, DO, WHILEEND, BEGIN, END, id };

class PRDP : public SymbolTable, public ErrorTable, public Tree<string> {
private:
    symbol table;
    ofstream output;
    string g, type_value;
    bool on;
    pair<size_t, string> backup;
    vector<string> rhs;

public:

    string next_lexeme(symbol);
    void copy(symbol&);
    void lexer();
    void viewTable();

    void printRules(NONTERMINAL);
    void printRules(TERMINAL);

    void getTree();
    Node<string>* getRoot();

    bool parser(symbol, bool);
    bool FIRST(NONTERMINAL);
    bool FOLLOW(NONTERMINAL);
    bool IF();
    bool WHILE();
    bool BEGIN();
    bool MS();
    bool MID();
    bool TY();
    bool ID();
    bool NUM();
    bool RELOP();
    bool A();
    bool B();
    bool C();
    bool D();
    bool E();
    bool F();
    bool Q();
    bool R();
    bool S();
    bool T();
};

void PRDP::getTree(){
    cout << "\n+--Parse Tree--+\n\n";
    printTree();
    cout << "\n+--------------+\n";
}

Node<string>* PRDP::getRoot(){
    return getRoot();
}

void PRDP::viewTable(){
    ICG();
}

bool PRDP::NUM(){
    return (token == "INTEGER" || token == "REAL") ? true : false;
}

bool PRDP::ID(){
    return (token == "IDENTIFIER") ? true : false;
}

bool PRDP::TY(){
    return (lexeme == "bool" || lexeme == "int" || lexeme == "float") ? true : false;
}

bool PRDP::RELOP(){
    return (lexeme == "<" || lexeme == "<=" || lexeme == "==" || lexeme == "<>" || lexeme == ">=" || lexeme == ">") ? true : false;
}

bool PRDP::FIRST(NONTERMINAL n){
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

void PRDP::printRules(TERMINAL t) {
    rhs.clear();
    if (on) {
        switch (t) {
            case LPAREN:
                output << "<F> -> ( <E> )" << endl;
                break;
            case RPAREN:
                output << write << "<F> -> )" << endl;
                break;
            default:
                output << write; 
                break;
        }
    }
}

void PRDP::printRules(NONTERMINAL n){
    rhs.clear();
    if (on){
        switch (n){
            case STATE:
                rhs.push_back("D");
                rhs.push_back("A");
                rhs.push_back("E");
                rhs.push_back("BEGIN");
                rhs.push_back("WHILE");
                rhs.push_back("IF");
                storeTree("S", rhs);
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
                storeTree("IF", rhs);
                output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                break;
            case COND:
                rhs.push_back("E");
                rhs.push_back("B");
                storeTree("C", rhs);
                output << "<C> -> <E> <B>\n" << write;
                break;
            case COND_PRIME:
                rhs.push_back("RELOP");
                rhs.push_back("E");
                storeTree("B", rhs);
                output << "<B> -> <RELOP> <E> | <empty>" << endl ;
                break;
            case RELA:
                rhs.push_back(lexeme);
                storeTree("RELOP", rhs);
                output << write << "<RELOP> -> < | <= | == | <> | >= | >" << endl;
                break;
            case WS:
                rhs.push_back("while");
                rhs.push_back("C");
                rhs.push_back("do");
                rhs.push_back("S");
                rhs.push_back("whilend");
                storeTree("WHILE", rhs);
                output << "<WHILE> -> while <C> do <S> whileend" << endl;
                break;
            case BS:
                rhs.push_back("begin");
                rhs.push_back("S");
                rhs.push_back("MS");
                rhs.push_back("end");
                storeTree("BEGIN", rhs);
                output << "<BEGIN> -> begin <S> <MS> end" << endl;
                break;
            case MRS:
                rhs.push_back(";");
                rhs.push_back("S");
                rhs.push_back("MS");
                storeTree("MS", rhs);
                output << write << "<MS> -> ; <S> <MS> | <empty>" << endl;
                break;
            case EXP:
                rhs.push_back("T");
                rhs.push_back("Q");
                storeTree("E", rhs);
                output << "<E> -> <T> <Q>" << endl;
                break;
            case EXP_PRIME:
                rhs.push_back(lexeme);
                rhs.push_back("T");
                rhs.push_back("Q");
                storeTree("Q", rhs);
                output << write << "<Q> -> + <T> <Q> | - <T> <Q> | <empty>" << endl;
                break;
            case TERM:
                rhs.push_back("F");
                rhs.push_back("R");
                storeTree("T", rhs);
                output << "<T> -> <F> <R>" << endl;
                break;
            case FACTOR_PRIME:
                rhs.push_back(lexeme);
                rhs.push_back("F");
                rhs.push_back("R");
                storeTree("R", rhs);
                output << write << "<R> -> * <F> <R> | / <F> <R> | <empty>" << endl;
                break;
            case FACTOR:
                if (lexeme == "(") {
                    rhs.push_back("(");
                    rhs.push_back("E");
                    rhs.push_back(")");
                }
                else
                    rhs.push_back(lexeme);
                storeTree("F", rhs);
                output << "<F> -> <ID> | <NUM> | ( <E> )" << endl;
                break;
            case NUMBER:
                rhs.push_back(lexeme);
                storeTree("NUM", rhs);
                output << "<NUM> -> " << lexeme << endl;
                break;
            case DECLARE:
                rhs.push_back("TY");
                rhs.push_back("ID");
                rhs.push_back("MID");
                rhs.push_back(";");
                storeTree("D", rhs);
                output << "<D> -> <TY> <ID> <MORE_IDS> ; | <empty>" << endl;
                break;
            case TYPE:
                output << "<TY> -> bool | int | float" << endl;
                rhs.push_back(lexeme);
                storeTree("TY", rhs);
                type_value = lexeme;
                break;
            case IDENTIFIER:
                rhs.push_back(lexeme);
                storeTree("ID", rhs);
                output << "<ID> -> " << lexeme << endl;
                insert(table.front());
                break;
            case MORE_ID:
                rhs.push_back(",");
                rhs.push_back("ID");
                rhs.push_back("MID");
                storeTree("MID", rhs);
                output << write << "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>" << endl;
                break;
            case ASSIGN:
                rhs.push_back("ID");
                rhs.push_back("=");
                rhs.push_back("E");
                rhs.push_back(";");
                storeTree("A", rhs);
                output << "<A> -> <ID> = <E> ;" << endl;
                break;
            case MSE:
                rhs.push_back("empty");
                storeTree("MS", rhs);
                output << "<MS> -> <empty>" << endl;
                break;
            case ME:
                rhs.push_back("empty");
                storeTree("MID", rhs);
                output << "<MORE_IDS> -> <empty>" << endl;
                break;
            case QE:
                rhs.push_back("empty");
                storeTree("Q", rhs);
                output << "<Q> -> <empty>" << endl;
                break;
            case RE:
                rhs.push_back("empty");
                storeTree("R", rhs);
                output << "<R> -> <empty>" << endl;
                break;
            case BE:
                rhs.push_back("empty");
                storeTree("B", rhs);
                output << "<B> -> <empty>" << endl;
                break;
            default:
                break;
        }
    }
}

bool PRDP::S() {
    printRules(STATE);
    if (IF() || WHILE() || BEGIN() || A() || D() || E())
        return true;
}

bool PRDP::IF() {
    if (FIRST(IS)){
        printRules(IS);
        lexer();
        if (FIRST(COND)){
            C();
            if (lexeme == "then"){
                printRules(THEN); 
                lexer();
                if (FIRST(STATE)){
                    S();
                    if (lexeme == "else"){
                        printRules(ELSE); 
                        lexer();
                        if (FIRST(STATE)){
                            S();
                            if (lexeme == "endif"){
                                printRules(ENDIF); 
                                lexer();
                                return true;
                            }
                            else {
                                handler(backup, "endif");
                                return false;
                            }
                        }
                        else {
                            handler(backup, "<S>");
                            return false;
                        }
                    }
                    else {
                        handler(backup, "else");
                        return false;
                    }
                }
                else {
                    handler(backup, "<S>");
                    return false;
                }
            }
            else {
                handler(backup, "then");
                return false;
            }
        }
        else {
            handler(backup, "<C>");
            return false;
        }
    }
    else
        return FOLLOW(IS) ? true : false;
}

bool PRDP::C(){
    printRules(COND);
    if (FIRST(EXP)){
        E();
        if (FIRST(COND_PRIME)){
            B();
            return true;
        }
        else {
            printRules(BE);
            return FOLLOW(COND_PRIME) ? true : false;
        }
    }
    else {
        handler(backup, "<E>");
        return false;
    }
}

bool PRDP::B(){
    printRules(COND_PRIME);
    if (FIRST(RELA)){
        RELOP();
        printRules(RELA);
        lexer();
        if (FIRST(EXP)){
            printRules(id);
            E();
            return true;
        }
        else {
            handler(backup, "<E>");
            return false;
        }
    }
    else {
        handler(backup, "<RELOP>");
        return false;
    }
}

bool PRDP::WHILE(){
    if (FIRST(WS)){
        printRules(WS);
        lexer();
        if (FIRST(COND)){
            C();
            if (lexeme == "do"){
                printRules(DO);
                lexer();
                if (FIRST(STATE)){
                    S();
                    if (lexeme == "whileend"){
                        printRules(WHILEEND);
                        lexer();
                        return true;
                    }
                    else {
                        handler(backup, "whileend");
                        return false;
                    }
                }
                else {
                    handler(backup, "<S>");
                    return false;
                }
            }
            else {
                handler(backup, "do");
                return false;
            }
        }
        else {
            handler(backup, "<C>");
            return false;
        }
    }
    else
        return false;
}

bool PRDP::BEGIN(){
    if (FIRST(BS)){
		printRules(BS);
		lexer();
		if (FIRST(STATE)){
			S();
			if (FIRST(MRS))
				MS();
            else if (FOLLOW(MRS))
                printRules(MSE);
			else {
			    handler(backup, "end");
				return false;
			}
			printRules(END); 
			lexer();
			return true;
		}
		else {
			handler(backup, "<S>");
			return false;
		}
    }
    else
        return false;
}

bool PRDP::MS(){
    printRules(MRS);
    lexer();
    if (FIRST(STATE)){
        S();
        if (FIRST(MRS)){
            MS();
            return true;
        }
        else
            return false;
    }
    else {
        handler(backup, "<S>");
        return false;
    }
}
bool PRDP::E(){
    if (FIRST(EXP)){
        printRules(EXP);
        if (FIRST(TERM)){
            T();
            if (FIRST(EXP_PRIME)){
                Q();
                return true;
            }
            else {
                printRules(QE);
                return FOLLOW(EXP_PRIME) ? true : false;
           }
        }
        else
            return false;
    }
    else
        return false;
}

bool PRDP::Q(){
    if (lexeme == "+" || lexeme == "-"){
        printRules(EXP_PRIME);
        lexer();
        if (FIRST(TERM)) {
            printRules(id);
            T();
            Q();
            return true;
        }
        else
            return false;
    }
    return false;
}

bool PRDP::T(){
    printRules(TERM);
    if (FIRST(FACTOR)){
        F();
        if (FIRST(FACTOR_PRIME)){
            R();
            return true;
        }
        else {
            printRules(RE);
            return FOLLOW(FACTOR_PRIME) ? true : false;
        }
    }
    else
        return false;
}

bool PRDP::R(){
    if (lexeme == "*" || lexeme == "/"){
        printRules(FACTOR_PRIME);
        lexer();
        if (FIRST(FACTOR)){
            printRules(id);
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

bool PRDP::F(){
    printRules(FACTOR);
    if (lexeme == "("){
        printRules(LPAREN);
        lexer();
        if (FIRST(EXP)){
            printRules(id);
            E();
            if (lexeme == ")"){
                printRules(RPAREN);
                lexer();
                return true;
            }
            else {
                handler(backup, ")");
                return false;
            }
        }
        else {
            handler(backup, "<E>");
            return false;
        }
    }
    else if (FIRST(IDENTIFIER)){
        printRules(IDENTIFIER);
        lexer();
        return true;
    }
    else if (FIRST(NUMBER)){
        printRules(NUMBER);
        lexer();
        return true;
    }
    else {
        handler(backup, "= | ( | <ID> | <NUM>");
        return false;
    }
}

bool PRDP::D(){
    if (!table.empty()){
        if (FIRST(TYPE)){
            TY();
            printRules(DECLARE);
            printRules(TYPE);
            lexer();
            if (FIRST(IDENTIFIER)){
                types = type_value;
                printRules(id);
                printRules(IDENTIFIER);
                lexer();
                if (table.empty())
                    return true;
                if (FIRST(MORE_ID)){
                    MID();
                    return true;
                }
                else if (FOLLOW(MORE_ID)){
                    printRules(ME);
                    printRules(SEMI_COLON);
                    lexer();
                    return true;
                }
                else {
                    printRules(ME);
                    return true;
                }
            }
            else {
                handler(backup, "<ID>");
                return false;
            }
        }
        else
            return FOLLOW(DECLARE) ? true : false;
    }
    else
        return false;
}

bool PRDP::MID(){
	printRules(MORE_ID);
	lexer();
	if (FIRST(IDENTIFIER)){
        types = type_value;
        printRules(id);
		printRules(IDENTIFIER);
		lexer();
        if (FIRST(MORE_ID))
    		MID();
        else if (FOLLOW(MORE_ID)){
            printRules(ME);
            printRules(COMMA);
            lexer();
            return true;
        }
        else {
            printRules(ME);
            return true;
        }
	}
	else {
        handler(backup, "<ID>");
        return false;
	}
}

bool PRDP::A(){
    if (!table.empty()){
        if (FIRST(IDENTIFIER)){
            if (next_lexeme(table) == "="){
                printRules(ASSIGN);
                printRules(IDENTIFIER);
                lexer();
                printRules(EQUAL);
                lexer();
                if (FIRST(EXP)){
                    printRules(id);
                    E();
                    if (lexeme == ";"){
                        printRules(SEMI_COLON); 
                        lexer();
                    }
                    return true;
                }
                else {
                    handler(backup, "<E>");
                    return false;
                }
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

void PRDP::lexer() {
    g = g + lexeme + " ";
    backup = { ln, lexeme };
    table.pop();
}

string PRDP::next_lexeme(symbol w){
    w.pop();
    return w.front().lex;
}
 
void PRDP::copy(symbol& w){
    while (!w.empty()) {
        table.push(w.front());
        w.pop();
    }
}

bool PRDP::parser(symbol w, bool b){
    system("cls");
    copy(w);
    on = b;
    output.open("files/out.txt");
    output << "+--START--+\n";

    if (table.empty()){
        cout << "\033[91m" << "WARNING: " << "\033[0m" << "reading empty file!" << endl;
        exit(0);
    }

    while (!table.empty()){
        g = "";
        if (S())
            output << "\n***ACCEPTED***" << " => " << g << endl;
        else
            output << "\n***NOT ACCEPTED***\n";
        if (FOLLOW(STATE))
            lexer();
    }

    output << "\n+--FINISH--+\n";
    output.close();
    
    if (!error() && empty()) {
        cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n";
        return true;
    }
    return false;
}
