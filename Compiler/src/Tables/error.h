#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <utility>

#define token table.front().tok
#define lexeme table.front().lex
#define line table.front().ln
#define type table.front().typ
#define scope table.front().s_ref

using namespace std;

struct Token {
   size_t ln;
   string tok;
   string lex;
   size_t mem;
   string typ;
   size_t s_ref;
};

typedef queue<Token> symbol;

class ErrorTable {
private:
   queue<string> e;
   
protected:
   string efile;

public:
   void handler(symbol, const string&, const size_t&, const size_t&);
   void handler(const size_t&, const string&);
   void handler(const Token&, const string&);
   void display();
   bool empty();
};

void ErrorTable::handler(const size_t& ln, const string& w){
   string temp;
   temp += w[0];

   if (empty())
      e.push("In file: " + efile + "\n\n");

   e.push("\033[91mINVALID TOKEN: \033[0m\"" + w + "\" on Line: " + to_string(ln) + "\n");
   
   for (int i = 0; i < 16; ++i)
      e.push(" ");
   e.push("^");

   for (int i = 0; i < w.length()-1; ++i)
      e.push("~");
   e.push("\n");
   
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){
            temp = "\t-> Expected Numeric Value at: \'";
            e.push(temp + W);
         }
      }
      if (w.find('.') != string::npos)
         e.push("\t-> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_')
      e.push("\t-> Identifiers cannot start with \'" + temp);
   e.push("\'\n\n");
}

void ErrorTable::handler(const Token& p, const string& q){
      if (empty())
         e.push("In file: " + efile + "\n\n");
      e.push("\033[91m");
      
      if (q == "<E>")
         e.push("INVALID EXPRESSION");
      else if (q == "<ID>")
         e.push("INVALID IDENTIFIER");
      else if (q == ")")
         e.push("INVALID SEPARATOR");
      else if (q == "<S>")
         e.push("INVALID SENTENCE");
      else if (q == ")")
         e.push("INVALID SEPARATOR");
      else if (q == "<C>")
         e.push("INVALID CONDITIONAL");
      else
         e.push("INVALID TOKEN");

      e.push("\033[0m on line: " + to_string(p.ln) + "\n\t-> expected \'" + q + "\' after \'" + p.lex + "\'\n");

      for (int i = 0; i < 21; ++i)
         e.push(" ");
      e.push("^");

      for (int i = 0; i < q.length()-1; ++i)
         e.push("~");
      e.push("\n\n");

      display();
}

void ErrorTable::handler(symbol table, const string& f, const size_t& s, const size_t& i){
   if (empty())
      e.push("In file: " + f + "\n\n");
   switch (i) {
      case 1:
         e.push("\033[91mINVALID DECLARATION\033[0m on line: " + to_string(line) + "\n\t-> \'" + lexeme + "\' has already been declared previously on line: " + to_string(s) + "\n");
         break;
      case 2:
         e.push("\033[91mUNDEFINED TOKEN\033[0m on line: " + to_string(line) + "\n\t-> \'" + lexeme + "\' was not declared in the scope\n");
         break;
      case 3:
         e.push("\033[91mOUT OF SCOPE\033[0m on line: " + to_string(line) + "\n\t-> \'" + lexeme + "\' reference is out of scope\n");
         break;
      case 4:
         e.push("\033[91mINVALID TYPE\033[0m on line: " + to_string(line) + "\n\t-> \'" + lexeme + "\' type value does not match with its declaration\n");
         break;
      default:
         break;
   }

   for (int i = 0; i < 12; ++i)
      e.push(" ");
   e.push("^");
   
   for (int i = 0; i < lexeme.length()-1; ++i)
      e.push("~");
   e.push("\n\n");
}

void ErrorTable::display(){
   while (!empty()){
      cout << e.front();
      e.pop();
   }
}

bool ErrorTable::empty(){
   return e.empty() ? true : false;
}
