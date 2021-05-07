#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>
#include <stack>
#include "../Tables/symbol.h"
#include "../Tables/error.h"
#include "../Tables/tree.h"

#define token table.front().tok
#define lexeme table.front().lex
#define ln table.front().ln
#define type table.front().typ
#define scope table.front().s_ref
#define write "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"

using namespace std;

typedef queue<Token> symbol;

enum NONTERMINAL { STATE, DECLARE, MORE_ID, ME, TYPE, ASSIGN, EXP, EXP_PRIME, QE, TERM, FACTOR, FACTOR_PRIME, RE, IDENTIFIER, NUMBER, IS, WS, BS, BE, MRS, MSE, COND, COND_PRIME, RELA };
enum TERMINAL { SEMI_COLON, COMMA, LPAREN, RPAREN, EQUAL, THEN, ELSE, ENDIF, DO, WHILEEND, BEGIN, END, id };

class PRDP : public SymbolTable, public ErrorTable, public Tree<string> {
private:
    symbol table;
    stack<size_t> jump_stack;
    Token save, backup;
    bool on;
    ofstream output;
    char OP;
    string CURR_TYPE, SAVE_TYPE;
    size_t CURR_SCOPE, SAVE_ADDR;

public:
    PRDP(string f){
        efile = f;
        sfile = f;
        CURR_SCOPE = 0;
        SAVE_ADDR = 0;
        CURR_TYPE = "";
        SAVE_TYPE = "";
    }

    string next_lexeme(symbol);
    void copy(symbol&);
    void lexer();
    void viewTable();
    void backPatch(const size_t&);

    void printRules(NONTERMINAL);
    void printRules(TERMINAL);
    void printAL();

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

void PRDP::backPatch(const size_t& jump_addr){
    SAVE_ADDR = jump_stack.top();
    find(SAVE_ADDR).mem = jump_addr;
}

void PRDP::getTree(){
    cout << "+--Parse Tree--+\n\n";
    printTree();
    cout << "\n+--------------+\n";
}

Node<string>* PRDP::getRoot(){
    return getRoot();
}

void PRDP::viewTable(){
    sprint();
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
        case DECLARE:
            return (TY()) ? true : false;
        case MORE_ID:
            return (lexeme == ",") ? true : false;
        case TYPE:
            return (TY()) ? true : false;
        case ASSIGN:
            return (ID()) ? true : false;
        case EXP:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
        case EXP_PRIME:
            return (lexeme == "+" || lexeme == "-") ? true : false;
        case TERM:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
        case FACTOR:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
        case FACTOR_PRIME:
            return (lexeme == "*" || lexeme == "/") ? true : false;
        case IDENTIFIER:
            return (ID()) ? true : false;
        case NUMBER:
            return (NUM()) ? true : false;
        case IS:
            return (lexeme == "if") ? true : false;
        case WS:
            return (lexeme == "while") ? true : false;
        case BS:
            return (lexeme == "begin") ? true : false;
        case MRS:
            return (lexeme == ";") ? true : false;
        case COND:
            return (lexeme == "(" || ID() || NUM()) ? true : false;
        case COND_PRIME:
            return (RELOP()) ? true : false;
        case RELA:
            return (RELOP()) ? true : false;
        default:
            return false;
    }
}

bool PRDP::FOLLOW(NONTERMINAL n){
    if (table.empty())
        return false;
    switch (n){
        case STATE:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case DECLARE:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case MORE_ID:
            return (lexeme == ";") ? true : false;
        case TYPE:
            return (ID()) ? true : false;
        case ASSIGN:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case EXP:
            return (lexeme == "%" || lexeme == "else" || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
        case EXP_PRIME:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
        case TERM:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-") ? true : false;
        case FACTOR:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/") ? true : false;
        case FACTOR_PRIME:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-") ? true : false;
        case IDENTIFIER:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "=" || lexeme == ",") ? true : false;
        case NUMBER:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do" || lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/") ? true : false;
        case IS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case WS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case BS:
            return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end") ? true : false;
        case MRS:
            return (lexeme == "end") ? true : false;
        case COND:
            return (lexeme == "then" || lexeme == "do") ? true : false;
        case COND_PRIME:
            return (lexeme == "then" || lexeme == "do") ? true : false;
        case RELA:
             return (lexeme == "%" || lexeme ==  "else"  || lexeme == "endif" || lexeme == "whileend" || lexeme == ";" || lexeme == "end" || lexeme == ")" || RELOP() || lexeme == "then" || lexeme == "do") ? true : false;
        default:
            return false;
    }
}

void PRDP::printAL(){
    aprint();
}

void PRDP::printRules(TERMINAL t) {
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
    if (on){
        switch (n){
            case STATE:
                makeTree("S", vector<string> { "D", "A", "E", "BEGIN", "WHILE", "IF" });
                output << write << "<S> -> <A> | <D> | <E> | <IF> | <WHILE> | <BEGIN>" << endl;
                break;
            case IS:
                makeTree("IF", vector<string> { "if", "C", "then", "else", "S", "endif" });
                output << "<IF> -> if <C> then <S> else <S> endif" << endl;
                break;
            case COND:
                makeTree("C", vector<string> { "E", "B" });
                output << "<C> -> <E> <B>\n" << write;
                break;
            case COND_PRIME:
                makeTree("B", vector<string> { "RELOP", "E" });
                output << "<B> -> <RELOP> <E> | <empty>" << endl ;
                break;
            case RELA:
                makeTree("RELOP", vector<string> { lexeme });
                output << write << "<RELOP> -> < | <= | == | <> | >= | >" << endl;
                break;
            case WS:
                makeTree("WHILE", vector<string> { "while", "C", "do", "S", "whileend" });
                output << "<WHILE> -> while <C> do <S> whileend" << endl;
                break;
            case BS:
                makeTree("BEGIN", vector<string> { "begin", "S", "MS", "end" });
                output << "<BEGIN> -> begin <S> <MS> end" << endl;
                break;
            case MRS:
                makeTree("MS", vector<string> { ";", "S", "MS" });
                output << write << "<MS> -> ; <S> <MS> | <empty>" << endl;
                break;
            case EXP:
                makeTree("E", vector<string> { "T", "Q" });
                output << "<E> -> <T> <Q>" << endl;
                break;
            case EXP_PRIME:
                makeTree("Q", vector<string> { lexeme, "T", "Q" });
                output << write << "<Q> -> + <T> <Q> | - <T> <Q> | <empty>" << endl;
                break;
            case TERM:
                makeTree("T", vector<string> { "F", "R" });
                output << "<T> -> <F> <R>" << endl;
                break;
            case FACTOR_PRIME:
                makeTree("R", vector<string> { lexeme, "F", "R" });
                output << write << "<R> -> * <F> <R> | / <F> <R> | <empty>" << endl;
                break;
            case FACTOR:
                if (lexeme == "(")
                    makeTree("F", vector<string> { "(", "E", ")" });
                else if (NUM())
                    makeTree("F", vector<string> { "ID" });
                else
                    makeTree("F", vector<string> { "NUM" });

                output << "<F> -> <ID> | <NUM> | ( <E> )" << endl;
                break;
            case NUMBER:
                makeTree("NUM", vector<string> { lexeme });
                output << "<NUM> -> " << lexeme << endl;
                break;
            case DECLARE:
                makeTree("D", vector<string> { "TY", "ID", "MID", ";" });
                output << "<D> -> <TY> <ID> <MORE_IDS> ; | <empty>" << endl;
                break;
            case TYPE:
                CURR_TYPE = lexeme;
                makeTree("TY", vector<string> { lexeme });
                output << "<TY> -> bool | int | float" << endl;
                break;
            case IDENTIFIER:
                insert(table, CURR_SCOPE);
                makeTree("ID", vector<string> { lexeme });
                output << "<ID> -> " << lexeme << endl;
                break;
            case MORE_ID:
                makeTree("MID", vector<string> { ",", "ID", "MID" });
                output << write << "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>" << endl;
                break;
            case ASSIGN:
                makeTree("A", vector<string> { "ID", "=", "E", ";" });
                output << "<A> -> <ID> = <E> ;" << endl;
                break;
            case MSE:
                makeTree("MS", vector<string> { "empty" });
                output << "<MS> -> <empty>" << endl;
                break;
            case ME:
                makeTree("MID", vector<string> { "empty" });
                output << "<MORE_IDS> -> <empty>" << endl;
                break;
            case QE:
                makeTree("Q", vector<string> { "empty" });
                output << "<Q> -> <empty>" << endl;
                break;
            case RE:
                makeTree("R", vector<string> { "empty" });
                output << "<R> -> <empty>" << endl;
                break;
            case BE:
                makeTree("B", vector<string> { "empty" });
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
        CURR_SCOPE++;
        SAVE_ADDR = aLoc;
        printRules(IS);
        lexer();
        if (FIRST(COND)){
            C();
            if (lexeme == "then"){
                printRules(THEN); 
                lexer();
                if (FIRST(STATE)){
                    S();
                    backPatch(aLoc);
                    if (lexeme == "else"){
                        printRules(ELSE);
                        lexer();
                        if (FIRST(STATE)){
                            S();
                            backPatch(aLoc);
                            if (lexeme == "endif"){
                                CURR_SCOPE--;
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
    SAVE_TYPE = "";
    if (FIRST(EXP)){
        E();
        if (FIRST(COND_PRIME)){
            B();
            SAVE_TYPE = "";
            return true;
        }
        else {
            SAVE_TYPE = "";
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
        OP = lexeme[0]; // gets the operator as a <char>
        lexer();
        if (FIRST(EXP)){
            printRules(id);
            E();
            switch (OP){
                case '<':
                    getInstr("LES", 0);
                    jump_stack.push(aLoc);
                    getInstr("JUMPZ", 0);
                    break;
                case '>':
                    getInstr("GRT", 0);
                    jump_stack.push(aLoc);
                    getInstr("JUMPZ", 0);
                    break;
                default:
                    break;
            }
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
        CURR_SCOPE++;
        SAVE_ADDR = aLoc;
        getInstr("LABEL", 0);
        printRules(WS);
        lexer();
        if (FIRST(COND)){
            C();
            if (lexeme == "do"){
                printRules(DO);
                lexer();
                if (FIRST(STATE)){
                    S();
                    getInstr("JUMP", SAVE_ADDR);
                    backPatch(aLoc);
                    if (lexeme == "whileend"){
                        CURR_SCOPE--;
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
        CURR_SCOPE++;
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
            CURR_SCOPE--;
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
            getInstr("ADD", 0);
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
            getInstr("MUL", 0);
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
        SAVE_TYPE = inType("IDENTIFIER", SAVE_TYPE, table);
        getInstr("PUSHM", find(list, lexeme).mem);
        lexer();
        return true;
    }
    else if (FIRST(NUMBER)){
        printRules(NUMBER);
        SAVE_TYPE = inType("NUMBER", SAVE_TYPE, table);
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
                type = CURR_TYPE;
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
        type = CURR_TYPE;
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
    if (FIRST(IDENTIFIER)){
        save = table.front();
        if (next_lexeme(table) == "="){
            printRules(ASSIGN);
            printRules(IDENTIFIER);
            SAVE_TYPE = getType(lexeme);
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
                SAVE_TYPE = "";
                getInstr("POPM", find(list, save.lex).mem); 
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

void PRDP::lexer(){
    backup = table.front();
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
        if (S())
            output << "\n***ACCEPTED***\n";
        else
            output << "\n***NOT ACCEPTED***\n";
        if (FOLLOW(STATE))
            lexer();
    }

    output << "\n+--FINISH--+\n";
    output.close();
    
    return (error() || !empty()) ? false : true;
}
