#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>
#include "lexer.h"
#include "error_table.h"

#define token w.front().first
#define lexeme w.front().second

using namespace std;

typedef queue<pair<string, string>> symbol;

class Syntax {
private:
    ofstream output;
    string g, e;
    symbol temp;

public:
    string next_token(symbol);
    void add(symbol&);
    void write(symbol&, string);
    void terminate();

    void RDP(symbol);
    bool IF(symbol&);
    bool WHILE(symbol&);
    bool BEGIN(symbol&);
    bool MS(symbol&);
    bool MID(symbol&);
    bool TYPE(symbol&);
    bool ID(symbol&);
    bool NUM(symbol&);
    bool RELOP(symbol&);
    bool A(symbol&);
    bool C(symbol&);
    bool D(symbol&);
    bool E(symbol&);
    bool F(symbol&);
    bool Q(symbol&);
    bool R(symbol&);
    bool S(symbol&);
    bool T(symbol&);
};

bool Syntax::NUM(symbol& w) {
    return (token == "INTEGER" || token == "REAL") ? true : false;
}

bool Syntax::ID(symbol& w) {
    return (token == "IDENTIFIER") ? true : false;
}

bool Syntax::TYPE(symbol& w) {
    return (lexeme == "bool" || lexeme == "int" || lexeme == "float") ? true : false;
}

bool Syntax::RELOP(symbol& w) {
    return (lexeme == "<" || lexeme == "<=" || lexeme == "==" || lexeme == "<>" || lexeme == ">=" || lexeme == ">") ? true : false;
}

bool Syntax::S(symbol& w) {
    output << "<S> -> <A> | <D> | <E> | <IF> | <WHILE> | <BEGIN>" << endl;
    return IF(w) || WHILE(w) || BEGIN(w) || A(w) || D(w) || E(w) ? true : false;
}

bool Syntax::IF(symbol& w) {
    if (lexeme == "if") {
        output << "<IF> -> if <C> then <S> else <S> endif" << endl;
        add(w);
        if (C(w)) {
            if (lexeme == "then") {
                write(w, "");
                add(w);
                write(w, "");
                if (S(w)) {
                    if (lexeme == "else") {
                        write(w, "");
                        add(w);
                        write(w, "");
                        if (S(w)) {
                            if (lexeme == "endif") {
                                write(w, "");
                                add(w);
                                return true;
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
                else
                    return false;
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

bool Syntax::C(symbol& w) {
    output << "<C> -> <E> <RELOP> <E> | <E>" << endl;
    write(w, "");
    if (E(w)) {
        if (RELOP(w)) {
            write(w, "");
            output << "<RELOP> -> " << lexeme << endl;
            add(w);
            write(w, "");
            if (E(w))
                return true;
            else
                return false;
        }
        else {
            output << "<C> -> <E>" << endl;
            return true;
        }
    }
    else
        return false;
}

bool Syntax::WHILE(symbol& w) {
    if (lexeme == "while") {
        output << "<WHILE> -> while <C> do <S> whileend" << endl;
        add(w);
        if (C(w)) {
            if (lexeme == "do") {
                write(w, "");
                add(w);
                write(w, "");
                if (S(w)) {
                    if (lexeme == "whileend") {
                        write(w, "");
                        add(w);
                        return true;
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
        else
            return false;
    }
    else
        return false;
}

bool Syntax::BEGIN(symbol& w) {
    if (lexeme == "begin") {
        output << "<BEGIN> -> begin <S> <MS> end" << endl;
        add(w);
        write(w, "");
        if (S(w)) {
            if (MS(w)) {
                if (lexeme == "end") {
                    write(w, "");
                    add(w);
                    return true;
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
    else
        return false;
}

bool Syntax::MS(symbol& w) {
    if (lexeme == ";") {
        write(w, "");
        output << "<MS> -> ; <S> <MS> | <empty>" << endl;
        add(w);
        write(w, "");
        if (S(w)) {
            if (MS(w))
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else {
        output << "<MS> -> <empty>" << endl;
        return true;
    }
}

bool Syntax::E(symbol& w) {
    output << "<E> -> <T> <Q>" << endl;
    if (T(w)) {
        if (Q(w))
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Syntax::Q(symbol& w) {
    if (lexeme == "+" || lexeme == "-") {
        write(w, "<Q> -> " + lexeme + " <T> <Q> | <empty>\n");
        add(w);
        write(w, "");
        if (T(w)) {
            if (Q(w))
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else {
        output << "<Q> -> <empty>" << endl;
        return true;
    }
}

bool Syntax::T(symbol& w) {
    output << "<T> -> <F> <R>" << endl;
    if (F(w)) {
        if (R(w))
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Syntax::R(symbol& w) {
    if (lexeme == "*" || lexeme == "/") {
        write(w, " <R> -> " + lexeme + " <F> <R> | <empty>\n");
        add(w);
        write(w, "");
        if (F(w)) {
            if (R(w))
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else {
        output << "<R> -> <empty>" << endl;
        return true;
    }
}

bool Syntax::F(symbol& w) {
    if (lexeme == "(") {
        output << "<F> -> ( <E>" << endl;
        add(w);
        write(w, "");
        if (E(w)) {
            if (lexeme == ")") {
                write(w, "<F> -> ( <E> )\n");
                add(w);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else if (ID(w)) {
        output << "<F> -> <ID>\n<ID> -> " << lexeme << endl;
        add(w);
        return true;
    }
    else if (NUM(w)) {
        output << "<F> -> <NUM>\n<NUM> -> " << lexeme << endl;
        add(w);
        return true;
    }
    else
        return false;
}

bool Syntax::D(symbol& w) {
    if (TYPE(w)) {
        output << "<D> -> <TYPE> <ID> <MORE_IDS> ; | <empty>\n<TYPE> -> " << lexeme << endl;
        add(w);
        write(w, "");
        if (ID(w)) {
            output << "<ID> -> " << lexeme << endl;
            add(w);
            MID(w);
            if (lexeme == ";") {
                write(w, "");
                add(w);
            }
            return true;
        }
        else {
            e += "<ID> | ";
            return false;
        }
    }
    else
        return false;
}

bool Syntax::MID(symbol& w) {
    if (lexeme == ",") {
        g = g + lexeme + " ";
        write(w, "<MORE_IDS> -> , <ID> <MORE_IDS> | <empty>\n");
        w.pop();
        if (ID(w)) {
            write(w, "<ID> -> " + lexeme + "\n");
            add(w);
            MID(w);
        }
        else {
            e += "<ID> | ";
            return false;
        }
    }
    else {
        output << "<MORE_IDS> -> <empty>" << endl;
        return true;
    }
}

bool Syntax::A(symbol& w) {
    if (ID(w)) {
        if (next_token(w) == "=") {
            output << "<A> -> <ID> = <E> ;\n<ID> -> " << lexeme << endl;
            add(w);
            write(w, "");
            add(w);
            write(w, "");
            if (E(w)) {
                if (lexeme == ";") {
                    write(w, "");
                    add(w);
                }
                return true;
            }
            else {
                e += "<E> | ";
                return false;
            }
        }
        else
            return false;
    }
    else
        return false;
}

void Syntax::add(symbol& w) {
    g = g + lexeme + " ";
    w.pop();
}

void Syntax::write(symbol& w, string alpha) {
    output << "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n" << alpha;
}

string Syntax::next_token(symbol w) {
    w.pop();
    return lexeme;
}

void Syntax::RDP(symbol table) {
    system("cls");
    output.open("out.txt");

    while (!table.empty()) {
        g = "";
        write(table, "");
        if (S(table))
            output << "\n***ACCEPTED***" << " => " << g << endl;
        else
            terminate();
    }
    cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n";
    output.close();
}

void Syntax::terminate() {
    cout << "\033[91m" << "FAILED: " << "\033[0m" << " expected => " << e <<  "view in out.txt\n";
    output << "\n***NOT ACCEPTED***";
    output.close();
    system("pause");
    exit(0);
}
