#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <queue>
#include <stack>
#include <utility>

#include "symbol_table.h"
#include "error_table.h"

using namespace std;

typedef queue<pair<string, string>> symbol;

class Lexer {
private:
   ErrorTable e;
   SymbolTable sym;
   size_t state, col;
   string buffer,
          s = "(){}[],.:;",
          o = "*+-=/><%";

   vector<string> k {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};
   queue<pair<int, string>> q;
   vector<vector<size_t>> tState
       {{ 1, 2, 3, 8, 7, 4, 5, 0, 6 },
        { 1, 1, 8, 1, 7, 4, 5, 0, 6 },
        { 8, 2, 3, 8, 7, 4, 5, 0, 6 },
        { 8, 3, 8, 8, 7, 4, 5, 0, 6 },
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
        { 7, 7, 7, 7, 0, 7, 7, 7, 7 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

public:
   void FSM(const string&);
   int getCol(const char&);
   int getSign(const char&);
   bool isD(const char&);
   bool isK(const string&);
   bool wasSOP();
   void reset();
   void display();
   void viewTable();
   symbol getTable();
};

bool Lexer::isK(const string& w){
   for (string K : k){
      if (K == w){
         q.front().first--; 
         return true;
      }
   }
   return false; 
}

bool Lexer::isD(const char& c){
   return (c == ' ' || c == '\t') ? true : false; 
}                                                 

bool Lexer::wasSOP(){
   return (state == 4 || state == 5 || state == 6) ? true : false;
}

int Lexer::getSign(const char& c){        
   if (c == '_' || c == '$')  return 1;   
   if (c == '!')              return 2;   

   for (char S : s){                      
      if (S == c)
         return (S == '.') ? 0 : 5;       
   }

   for (char O : o){                      
      if (O == c)
         return 6;
   }
}

int Lexer::getCol(const char& c){
   int signs = getSign(c);
   if      (isalpha(c))    return 0;    
   else if (isdigit(c))    return 1;    
   else if (signs == 0)    return 2;    
   else if (signs == 1)    return 3;    
   else if (signs == 2)    return 4;    
   else if (signs == 5 || signs == 6){  
      if (!wasSOP())                    
         reset();
      return signs;                     
   }

   else if (isD(c)){                    
      if (!wasSOP())
         q.push({state, buffer});
      return 7;
   }

   else {
      if (!wasSOP())
         reset();
      return 8; 
   }
}

void Lexer::FSM(const string& w){
   buffer = "", state = 0, col = 0; 
   e.inc();                     

   for (size_t i = 0; i < w.length(); ++i){  
      col = getCol(w[i]);                    
      state = tState[state][col];            
      buffer += w[i];                        

      if (isD(w[i]))
         buffer = "";     
      else if (wasSOP())  
         reset();
      else if (state == 8)
         e.handler(buffer); 
   }

   if (!wasSOP())
      q.push({state, buffer}); 
}

void Lexer::display(){
   system("cls");

   if (e.empty()){ 
      string line, border = "+------------------------------+\n";
      line.assign(28, '-'); 
      cout << border << "| TOKENS" << setw(25) << "LEXEMES |\n" << border; 

      while (!q.empty()){
         switch (q.front().first){ 
            case 1:
               if (isK(q.front().second)){
                  cout << right << "| KEYWORD" << setw(8) << "=" << left << right << setw(13) << q.front().second << " |" << endl; 
                  sym.insert("KEYWORD", q.front().second);
               }
               else {
                  cout << right << "| IDENTIFIER" << setw(5) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
                  sym.insert("IDENTIFIER", q.front().second); 
               }
               break;
            case 2:
               cout << right << "| INTEGER" << setw(8) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               sym.insert("INTEGER", q.front().second);
               break;
            case 3:
               cout << right << "| REAL" << setw(11) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               sym.insert("REAL", q.front().second);
               break;
            case 4:
               cout << right << "| SEPARATOR" << setw(6) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               sym.insert("SEPARATOR", q.front().second);
               break;
            case 5:
               cout << right << "| OPERATOR" << setw(7) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               sym.insert("OPERATOR", q.front().second);
               break;
            case 6: 
               cout << right << "| PUNCTUATION" << setw(4) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               sym.insert("PUNCTUATION", q.front().second);
               break;
            default:
               break;
         }
         q.pop();
      }
      cout << border;
   }
   else
      e.display();
}

void Lexer::reset(){ 
   q.push({state, buffer});
   buffer = "";
}

symbol Lexer::getTable(){
   return sym.getTable();
}

void Lexer::viewTable(){
   sym.display();
}
