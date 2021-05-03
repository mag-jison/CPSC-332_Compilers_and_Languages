#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iomanip>
#include "error.h"

using namespace std;

struct Token {
   size_t ln;
   string tok;
   string lex;
   size_t mem;
   string ttype;
   size_t s_ref;
};

typedef queue<Token> symbol;

class SymbolTable {
private:
   ErrorTable e;
   symbol table;
   Token save;
   size_t mLoc;

protected:
   string sfile;

public:
   SymbolTable(){
      this->mLoc = 5000;
   }
   
   symbol getTable();
   bool error();
   bool exist(symbol, const string&);
   bool inScope(symbol, const string&, const size_t&);
   void insert(const struct Token&, const string&, const size_t&);
   void insert(const size_t&, const string&, const string&);
   void sprint();
};

symbol SymbolTable::getTable(){
   return table;
}

bool SymbolTable::error(){
   if (!e.empty()){
      e.display();
      return true;
   }
   return false;
}

bool SymbolTable::exist(symbol temp, const string& l){
   while (!temp.empty()){
      if (temp.front().lex == l){
         save = temp.front();
         return true;
      }
      temp.pop();
   }
   return false;
}

bool SymbolTable::inScope(symbol temp, const string& l, const size_t& scope_ref){
   while (!temp.empty()){
      if (temp.front().lex == l)
         return (temp.front().s_ref <= scope_ref) ? true : false;
      temp.pop();
   }
}

void SymbolTable::insert(const struct Token& t, const string& ty, const size_t& scope_ref){
   if (t.ttype != ""){ // declaration statement; not in table yet
      if (!exist(table, t.lex))
         table.push({t.ln, t.tok, t.lex, mLoc++, t.ttype, scope_ref}); 
      else
         e.handler(t.lex, t.ln, sfile, save.ln, 1);
   }
   else if (!exist(table, t.lex)){ // assignment statement
      e.handler(t.lex, t.ln, sfile, save.ln, 2); 
   }
   else if (!inScope(table, t.lex, scope_ref)){            
      e.handler(t.lex, t.ln, sfile, scope_ref, 3);
   }
}

void SymbolTable::insert(const size_t& ln, const string& token, const string& lexeme){
   table.push({ln, token, lexeme});
}

void SymbolTable::sprint(){
   system("cls");
   cout << "+----------------------------------------------------------------------+\n";
   cout << "|   TOKENS   |   LEXEMES   |    #    |    Memory Location   |   Type   |\n";
   cout << "|------------|-------------|---------|----------------------|----------|\n";
   while (!table.empty()){
      if (table.front().tok == "IDENTIFIER")
         cout << "| " << table.front().tok << setw(13-table.front().tok.length()) << " | " << table.front().lex << setw(14-table.front().lex.length()) << " | " << table.front().ln << setw(10-(to_string(table.front().ln)).length()) << " | " << table.front().mem << setw(23-(to_string(table.front().mem)).length()) << " | " << table.front().ttype << setw(11-table.front().ttype.length()) << " | " << endl;
      table.pop();
   }
   cout << "+----------------------------------------------------------------------+";
}