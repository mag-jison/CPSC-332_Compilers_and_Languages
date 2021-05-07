#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iomanip>
#include <cmath>
#include "error.h"

#define token table.front().tok
#define lexeme table.front().lex
#define line table.front().ln
#define type table.front().typ
#define scope table.front().s_ref

using namespace std;

typedef queue<Token> symbol;

class SymbolTable {
private:
   ErrorTable e;
   symbol list;
   vector<string> scope_res;
   Token backup;
   size_t mLoc;

protected:
   string sfile;

public:
   SymbolTable(){
      this->mLoc = 5000;
      this->scope_res = {"GLOBAL", "INNER", "NESTED"};
   }
   
   symbol getTable();
   Token find(symbol, const string&);
   bool error();
   bool exist(symbol, const string&);
   bool inScope(const string&, const size_t&);
   string getScope(const size_t&);
   string inType(string, string, symbol);
   string getType(const string&);
   void insert(const symbol&, const size_t&);
   void insert(const size_t&, const string&, const string&);
   void sprint();
};

symbol SymbolTable::getTable(){
   return list;
}

bool SymbolTable::error(){
   if (!e.empty()){
      e.display();
      return true;
   }
   return false;
}

Token SymbolTable::find(symbol temp, const string& l){
   while (!temp.empty()){
      if (temp.front().lex == l)
         return temp.front();
      temp.pop();
   }
   return {0, "", "", 0, "", 0};
}

bool SymbolTable::inScope(const string& l, const size_t& scope_ref){
   return (find(list, l).s_ref <= scope_ref) ? true : false;
}

string SymbolTable::getScope(const size_t& s){
   return (s > 2) ? scope_res[2] : scope_res[s];
}

string SymbolTable::inType(string w, string SAVE_TYPE, symbol table){
   if (w == "IDENTIFIER"){
      if (SAVE_TYPE == ""){
         SAVE_TYPE = getType(lexeme);
      }
      else if (SAVE_TYPE != getType(lexeme)){
         e.handler(table, sfile, 0, 4);
      }
   }
   else if (w == "NUMBER"){
      if (token == "INTEGER"){
         if (SAVE_TYPE == "bool"){
               if (lexeme != "0" && lexeme != "1")
                  e.handler(table, sfile, 0, 4);
         }
         else if (SAVE_TYPE != "int")
            e.handler(table, sfile, 0, 4);
      }
      else if (token == "REAL"){
         if (SAVE_TYPE != "float")
            e.handler(table, sfile, 0, 4);
      }
      else if (SAVE_TYPE != getType(lexeme)){
         e.handler(table, sfile, 0, 4);
      }
   }
   return SAVE_TYPE;
}

string SymbolTable::getType(const string& l){
   return find(list, l).typ;
}

bool SymbolTable::exist(symbol temp, const string& l){
   while (!temp.empty()){
      if (temp.front().lex == l){
         backup = temp.front();
         return true;
      }
      temp.pop();
   }
   return false;
}

void SymbolTable::insert(const symbol& table, const size_t& scope_ref){
   if (type != ""){ // declaration statement; not in table yet
      if (!exist(list, lexeme))
         list.push({line, token, lexeme, mLoc++, type, scope_ref}); 
      else
         e.handler(table, sfile, backup.ln, 1);
   }
   else if (!exist(list, lexeme)){ // assignment statement
      e.handler(table, sfile, backup.ln, 2); 
   }
   else if (!inScope(lexeme, scope_ref)){
      e.handler(table, sfile, scope_ref, 3);
   }
}

void SymbolTable::insert(const size_t& ln, const string& tok, const string& lex){
   list.push({ln, tok, lex});
}

void SymbolTable::sprint(){
   system("cls");
   cout << "+----------------------------------------------------------------------------------+\n";
   cout << "|   TOKENS   |   LEXEMES   |    #    |    Memory Location   |   Type   |   Scope   |\n";
   cout << "|------------|-------------|---------|----------------------|----------|-----------|\n";
   while (!list.empty()){
      if (list.front().tok == "IDENTIFIER")
         cout << "| " << list.front().tok << setw(13-list.front().tok.length()) << " | " << list.front().lex << setw(14-list.front().lex.length()) << " | " << list.front().ln << setw(10-(to_string(list.front().ln)).length()) << " | " << list.front().mem << setw(23-(to_string(list.front().mem)).length()) << " | " << list.front().typ << setw(11-list.front().typ.length()) << " | " << getScope(list.front().s_ref) << setw(12-(getScope(list.front().s_ref).length())) << " | " << endl;
      list.pop();
   }
   cout << "+----------------------------------------------------------------------------------+";
}
