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
   string types;
};

typedef queue<Token> symbol;

class SymbolTable {
private:
   symbol table;
   ErrorTable e;
   size_t memory_address;
   string t, r, q;

public:
   SymbolTable(){
      this->memory_address = 5000;
   }
   
   symbol getTable();
   bool exist(symbol, const string&);
   void insert(const Token&);
   void insert(const size_t&, const string&, const string&);
   void ICG();
};

symbol SymbolTable::getTable(){
   return table;
}

bool SymbolTable::exist(symbol temp, const string& w){
   while (!temp.empty()){
      if (temp.front().lex == w)
         return true;
      temp.pop();
   }
   return false;
}

void SymbolTable::insert(const Token& t){
   if (t.tok == "IDENTIFIER"){
      if (t.types != ""){
         if (!exist(table, t.lex))
            table.push({t.ln, t.tok, t.lex, memory_address++, t.types}); 
         else
            e.handler(t.lex, 1);
      }
      else if (!exist(table, t.lex)){
         e.handler(t.lex, 2); 
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
         cout << "| " << table.front().tok << setw(13-table.front().tok.length()) << " | " << table.front().lex << setw(14-table.front().lex.length()) << " | " << table.front().ln << setw(10-(to_string(table.front().ln)).length()) << " | " << table.front().mem << setw(23-(to_string(table.front().mem)).length()) << " | " << table.front().types << setw(11-table.front().types.length()) << " | " << endl;
      table.pop();
   }
   cout << "+----------------------------------------------------------------------+";
}
