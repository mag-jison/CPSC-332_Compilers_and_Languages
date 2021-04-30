#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iomanip>
#include "error.h"

using namespace std;

struct Token { size_t ln; string tok; string lex; size_t mem; string ttype; };

typedef queue<Token> symbol;

class SymbolTable {
private:
   ErrorTable e;
   symbol table;
   size_t mLoc;
   string t, r, q;

public:
   SymbolTable(){
      this->mLoc = 5000;
   }
   
   symbol getTable();
   bool error();
   bool exist(symbol, const string&);
   void insert(const struct Token&);
   void insert(const size_t&, const string&, const string&);
   void ICG();
};

symbol SymbolTable::getTable(){
   return table;
}

bool SymbolTable::error(){
   if (e.empty())
      return false;
   else {
      e.display();
      return true;
   }
}

bool SymbolTable::exist(symbol temp, const string& w){
   while (!temp.empty()){
      if (temp.front().lex == w)
         return true;
      temp.pop();
   }
   return false;
}

void SymbolTable::insert(const struct Token& t){
   if (t.tok == "IDENTIFIER"){
      if (t.ttype != ""){
         if (!exist(table, t.lex))
            table.push({t.ln, t.tok, t.lex, mLoc++, t.ttype}); 
         else
            e.handler(t.lex, t.ln, 1);
      }
      else if (!exist(table, t.lex)){
         e.handler(t.lex, t.ln, 2); 
      }
   }
}

void SymbolTable::insert(const size_t& ln, const string& token, const string& lexeme){
   table.push({ln, token, lexeme});
}

void SymbolTable::ICG(){
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
