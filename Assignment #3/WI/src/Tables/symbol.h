#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iomanip>
#include "error.h"

#define token table.front().tok
#define lexeme table.front().lex
#define line table.front().ln
#define types table.front().ttype
#define scope table.front().s_ref

using namespace std;

typedef queue<Token> symbol;

class SymbolTable {
private:
   ErrorTable e;
   symbol list;
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
   string getScope(const size_t&);
   string inType(string, string, symbol);
   string getType(symbol, const string&);
   void insert(const symbol&, const string&, const size_t&);
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

bool SymbolTable::inScope(symbol temp, const string& l, const size_t& scope_ref){
   while (!temp.empty()){
      if (temp.front().lex == l)
         return (temp.front().s_ref <= scope_ref) ? true : false;
      temp.pop();
   }
}

string SymbolTable::getScope(const size_t& s){
   switch(s){
      case 0:
         return "GLOBAL";
         break;
      case 1:
         return "INNER";
         break;
      default:
         return "NESTED";
         break;
   }
}

string SymbolTable::inType(string w, string SAVE_TYPE, symbol table){
   if (w == "IDENTIFIER"){
      if (SAVE_TYPE == ""){
         SAVE_TYPE = getType(list, lexeme);
      }
      else if (SAVE_TYPE != getType(list, lexeme)){
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
      else if (SAVE_TYPE != getType(list, lexeme)){
         e.handler(table, sfile, 0, 4);
      }
   }
   return SAVE_TYPE;
}

string SymbolTable::getType(symbol temp, const string& l){
   while (!temp.empty()){
      if (temp.front().lex == l){
         return temp.front().ttype;
      }
      temp.pop();
   }
   return "";
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

void SymbolTable::insert(const symbol& table, const string& ty, const size_t& scope_ref){
   if (types != ""){ // declaration statement; not in table yet
      if (!exist(list, lexeme))
         list.push({ line, token, lexeme, mLoc++, types, scope_ref }); 
      else
         e.handler(table, sfile, save.ln, 1);
   }
   else if (!exist(list, lexeme)){ // assignment statement
      e.handler(table, sfile, save.ln, 2); 
   }
   else if (!inScope(list, lexeme, scope_ref)){
      e.handler(table, sfile, scope_ref, 3);
   }
}

void SymbolTable::insert(const size_t& lin, const string& tok, const string& lex){
   list.push({ lin, tok, lex });
}

void SymbolTable::sprint(){
   system("cls");
   cout << "+----------------------------------------------------------------------------------+\n";
   cout << "|   TOKENS   |   LEXEMES   |    #    |    Memory Location   |   Type   |   Scope   |\n";
   cout << "|------------|-------------|---------|----------------------|----------|-----------|\n";
   while (!list.empty()){
      if (list.front().tok == "IDENTIFIER")
         cout << "| " << list.front().tok << setw(13-list.front().tok.length()) << " | " << list.front().lex << setw(14-list.front().lex.length()) << " | " << list.front().ln << setw(10-(to_string(list.front().ln)).length()) << " | " << list.front().mem << setw(23-(to_string(list.front().mem)).length()) << " | " << list.front().ttype << setw(11-list.front().ttype.length()) << " | " << getScope(list.front().s_ref) << setw(12-(getScope(list.front().s_ref).length())) << " | " << endl;
      list.pop();
   }
   cout << "+----------------------------------------------------------------------------------+";
}