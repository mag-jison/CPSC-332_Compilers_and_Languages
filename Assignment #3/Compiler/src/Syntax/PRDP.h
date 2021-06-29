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
    bool on;                      // allows user to control output
    ofstream output;              // output file generator
    char OP;                      // stores the relational operator
    string CURR_TYPE, SAVE_TYPE;  // keeps track of the type value of the current identifier
    size_t CURR_SCOPE, SAVE_ADDR; // keeps track of the scope value of the current identifier

public:
    PRDP(string f)
	: CURR_SCOPE(0), SAVE_ADDR(0)
    {
        efile = f; // initializes the file-name in the ErrorTable whenever an error occurs
        sfile = f; // initializes the file-name in the SymbolTable whenever an error occurs
    }

    string next_lexeme(symbol);  // returns the next lexeme without disturbing the queue data structure
    void copy(symbol&);          // transfers the symbol table locally to the PRDP class
    void lexer();                // adds lexemes and gets next token and lexeme
    void viewTable();
    void backPatch(const size_t&);

    void printRules(NONTERMINAL);
    void printRules(TERMINAL);
    void printAL(); // returns the stored assembly code

    void getTree();
    Node<string>* getRoot();

    bool parser(symbol, bool); // predictive recursive descent parser (top-down parser)
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

void PRDP::backPatch(const size_t& jump_addr){
    SAVE_ADDR = jump_stack.top(); // assign the SAVE_ADDR to the top of the stack
    find(SAVE_ADDR).mem = jump_addr; // locates where the variable that holds the SAVE_ADDR and assign it to the new address
}

void PRDP::getTree(){
    printTree();
}

Node<string>* PRDP::getRoot(){
    return getRoot();
}

void PRDP::viewTable(){
    sprint();
}

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
    switch (n){
        case STATE: // makeTree will insert the current nonterminal (LHS) to the list of RHS nonterminal 
            makeTree("S", vector<string> { "D", "A", "E", "BEGIN", "WHILE", "IF" });
            if (on) // controls whether or not the user wants to print out the production rules
                output << write << "<S> -> <A> | <D> | <E> | <IF> | <WHILE> | <BEGIN>" << endl;
            break;
        case IS:
            makeTree("IF", vector<string> { "if", "C", "then", "else", "S", "endif" });
            if (on)
                output << "<IF> -> if <C> then <S> else <S> endif" << endl;
            break;
        case COND:
            makeTree("C", vector<string> { "E", "B" });
            if (on)
                output << "<C> -> <E> <B>\n" << write;
            break;
        case COND_PRIME:
            makeTree("B", vector<string> { "RELOP", "E" });
            if (on)
                output << "<B> -> <RELOP> <E> | <empty>" << endl;
            break;
        case RELA:
            makeTree("RELOP", vector<string> { lexeme });
            if (on)
                output << write << "<RELOP> -> < | <= | == | <> | >= | >" << endl;
            break;
        case WS:
            makeTree("WHILE", vector<string> { "while", "C", "do", "S", "whileend" });
            if (on)
                output << "<WHILE> -> while <C> do <S> whileend" << endl;
            break;
        case BS:
            makeTree("BEGIN", vector<string> { "begin", "S", "MS", "end" });
            if (on)
                output << "<BEGIN> -> begin <S> <MS> end" << endl;
            break;
        case MRS:
            makeTree("MS", vector<string> { ";", "S", "MS" });
            if (on)
                output << write << "<MS> -> ; <S> <MS> | <empty>" << endl;
            break;
        case EXP:
            makeTree("E", vector<string> { "T", "Q" });
            if (on)
                output << "<E> -> <T> <Q>" << endl;
            break;
        case EXP_PRIME:
            makeTree("Q", vector<string> { lexeme, "T", "Q" });
            if (on)
                output << write << "<Q> -> + <T> <Q> | - <T> <Q> | <empty>" << endl;
            break;
        case TERM:
            makeTree("T", vector<string> { "F", "R" });
            if (on)
                output << "<T> -> <F> <R>" << endl;
            break;
        case FACTOR_PRIME:
            makeTree("R", vector<string> { lexeme, "F", "R" });
            if (on)
                output << write << "<R> -> * <F> <R> | / <F> <R> | <empty>" << endl;
            break;
        case FACTOR:
            if (lexeme == "(")
                makeTree("F", vector<string> { "(", "E", ")" });
            else if (NUM())
                makeTree("F", vector<string> { "ID" });
            else
                makeTree("F", vector<string> { "NUM" });
            if (on)
                output << "<F> -> <ID> | <NUM> | ( <E> )" << endl;
            break;
        case NUMBER:
            makeTree("NUM", vector<string> { lexeme });
            if (on)
                output << "<NUM> -> " << lexeme << endl;
            break;
        case DECLARE:
            makeTree("D", vector<string> { "TY", "ID", "MID", ";" });
            if (on)
                output << "<D> -> <TY> <ID> <MORE_IDS> ; | <empty>" << endl;
            break;
        case TYPE:
            CURR_TYPE = lexeme; // saves the current type
            makeTree("TY", vector<string> { lexeme });
            if (on)
                output << "<TY> -> bool | int | float" << endl;
            break;
        case IDENTIFIER:
            insert(table, CURR_SCOPE); // inserts every identifier to the symbol table
            makeTree("ID", vector<string> { lexeme });
            if (on)
                output << "<ID> -> " << lexeme << endl;
            break;
        case MORE_ID:
            makeTree("MID", vector<string> { ",", "ID", "MID" });
            if (on)
                output << write << "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>" << endl;
            break;
        case ASSIGN:
            makeTree("A", vector<string> { "ID", "=", "E", ";" });
            if (on)
                output << "<A> -> <ID> = <E> ;" << endl;
            break;
        case MSE:
            makeTree("MS", vector<string> { "empty" });
            if (on)
                output << "<MS> -> <empty>" << endl;
            break;
        case ME:
            makeTree("MID", vector<string> { "empty" });
            if (on)
                output << "<MORE_IDS> -> <empty>" << endl;
            break;
        case QE:
            makeTree("Q", vector<string> { "empty" });
            if (on)
                output << "<Q> -> <empty>" << endl;
            break;
        case RE:
            makeTree("R", vector<string> { "empty" });
            if (on)
                output << "<R> -> <empty>" << endl;
            break;
        case BE:
            makeTree("B", vector<string> { "empty" });
            if (on)
                output << "<B> -> <empty>" << endl;
            break;
        default:
            break;
    }
}

bool PRDP::S() { // starting symbol
    printRules(STATE);
    if (IF() || WHILE() || BEGIN() || A() || D() || E())
        return true;
}

bool PRDP::IF() {
    if (FIRST(IS)){
        CURR_SCOPE++;     // increments the scope reference value
        SAVE_ADDR = aLoc; // assigns the current address to the SAVE_ADDR 
        printRules(IS);
        lexer();
        if (FIRST(COND)){
            C();
            if (lexeme == "then"){
                printRules(THEN); 
                lexer();
                if (FIRST(STATE)){
                    S();
                    backPatch(aLoc); // backPatch is needed in order to restore the memory address
                    if (lexeme == "else"){
                        printRules(ELSE);
                        lexer();
                        if (FIRST(STATE)){
                            S();
                            if (lexeme == "endif"){
                                CURR_SCOPE--; // decrements the scope reference value
                                printRules(ENDIF); 
                                lexer();
                                return true;
                            }
                            else {
                                handler(backup, "endif"); // handles errors if structure does not match
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
    SAVE_TYPE = ""; // save type is reseted since we are entering a new statement
    if (FIRST(EXP)){
        E();
        if (FIRST(COND_PRIME)){
            B();
            SAVE_TYPE = ""; // resets for the next statement
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
            switch (OP){ // will assign the appropriate instruction depending on the current input token
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
        SAVE_ADDR = aLoc; // saves the current address from the symbol table
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
    if (FIRST(EXP_PRIME)){
        OP = lexeme[0];
        printRules(EXP_PRIME);
        lexer();
        if (FIRST(TERM)) {
            printRules(id);
            T();
            switch(OP){
                case '+':
                    getInstr("ADD", 0); // zero is used in place of 'nil'
                    break;
                case '-':
                    getInstr("SUB", 0); // zero is used in place of 'nil'
                    break;
            }
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
    if (FIRST(FACTOR_PRIME)){
        OP = lexeme[0];
        printRules(FACTOR_PRIME);
        lexer();
        if (FIRST(FACTOR)){
            printRules(id);
            F();
            switch(OP){
                case '*':
                    getInstr("MUL", 0); //zero is used in place of 'nil'
                    break;
                case '/':
                    getInstr("DIV", 0); // zero is used in palce of 'nil'
                    break;
            }
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
        SAVE_TYPE = inType("IDENTIFIER", SAVE_TYPE, table); // assigns the SAVE_TYPE for future type checking within the same processing statement
        lexer();
        return true;
    }
    else if (FIRST(NUMBER)){
        printRules(NUMBER);
        SAVE_TYPE = inType("NUMBER", SAVE_TYPE, table); // assigns the SAVE_TYPE for future type checking within the same processing statement
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
