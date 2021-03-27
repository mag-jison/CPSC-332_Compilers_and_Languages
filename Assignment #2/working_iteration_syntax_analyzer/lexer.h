#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <queue>
#include <utility>

#include "symbol_table.h"
#include "error_table.h"

using namespace std;

typedef queue<pair<string, string>> symbol;
typedef vector<vector<size_t>> transition;

class Lexer {
private:
   ErrorTable e;
   SymbolTable sym;
   size_t state, col;
   string buffer, s, o;
   vector<string> k;
   queue<pair<int, string>> q;
   
   transition table {{ 1, 2, 3, 8, 7, 4, 5, 0, 6 },
                     { 1, 1, 8, 1, 7, 4, 5, 0, 6 },
                     { 8, 2, 3, 8, 7, 4, 5, 0, 6 },
                     { 8, 3, 8, 8, 7, 4, 5, 0, 6 },
                     { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
                     { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
                     { 1, 2, 3, 8, 7, 4, 5, 0, 6 },
                     { 7, 7, 7, 7, 0, 7, 7, 7, 7 },
                     { 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

public:
   Lexer();
   ~Lexer();
   void FSM(const string&);
   int getCol(const char&);
   int getSign(const char&);
   bool isD(const char&);
   bool isK(const string&);
   bool wasSOP();
   void reset();
   bool insert();
   void display();
   void viewTable();
   symbol request();
};

Lexer::Lexer(){
   this->k = {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};
   this->s = "(){}[],.:;";
   this->o = "*+-=/><%";
}

Lexer::~Lexer(){
   for (vector<size_t> element : table)
      element.clear();
}


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
      state = table[state][col];            
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

bool Lexer::insert(){
   int f;
   string s;

   if (e.empty()){ 
      while (!q.empty()){
         f = q.front().first;
         s = q.front().second;

         switch (f){ 
            case 1:
               isK(s) ? sym.insert("KEYWORD", s) : sym.insert("IDENTIFIER", s); 
               break;
            case 2:
               sym.insert("INTEGER", s);
               break;
            case 3:
               sym.insert("REAL", s);
               break;
            case 4:
               sym.insert("SEPARATOR", s);
               break;
            case 5:
               sym.insert("OPERATOR", s);
               break;
            case 6: 
               sym.insert("PUNCTUATION", s);
               break;
            default:
               break;
         }
         q.pop();
      }
      return true;
   }
   else {
      e.display();
      return false;
   }
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

symbol Lexer::request(){
   return sym.getTable();
}

void Lexer::viewTable(){
   sym.display();
}
