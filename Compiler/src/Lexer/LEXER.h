#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <queue>
#include <utility>
#include "../Tables/symbol.h"
#include "../Tables/error.h"

using namespace std;

typedef queue<Token> symbol;
typedef vector<vector<size_t>> transition;

struct V { 
   size_t ln;
   size_t state;
   string lex;
};

class LEXER : public SymbolTable, public ErrorTable {
private:
   size_t state, ln, col;
   string buffer, s, o;
   vector<string> k;
   queue<V> q;

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
   LEXER(string f) {
      efile = f;
      this->ln = 0;
      this->k = {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};
      this->s = "(){}[],.:;";
      this->o = "*+-=/><%";
   }

   void FSM(const string&);
   int getCol(const char&);
   int getSign(const char&);
   bool isD(const char&);
   bool isK(const string&);
   bool wasSOP();
   void reset();
   bool store();
   void printToken();
   symbol request();
};

bool LEXER::isK(const string& w){
   for (string K : k){
      if (K == w){
         q.front().state--;
         return true;
      }
   }
   return false;
}

bool LEXER::isD(const char& c){
   return (c == ' ' || c == '\t') ? true : false;
}

bool LEXER::wasSOP(){
   return (state == 4 || state == 5 || state == 6) ? true : false;
}

int LEXER::getSign(const char& c){
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
   return -1;
}

int LEXER::getCol(const char& c){
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
         q.push({ln, state, buffer});
      return 7;
   }

   else {
      if (!wasSOP())
         reset();
      return 8;
   }
}

void LEXER::FSM(const string& w){
   buffer = "", state = 0, col = 0, ln++;

   for (size_t i = 0; i < w.length(); ++i){
      col = getCol(w[i]);
      state = table[state][col];
      buffer += w[i];

      if (isD(w[i]))
         buffer = "";
      else if (wasSOP())
         reset();
      else if (state == 8)
         handler(ln, buffer);
   }

   if (!wasSOP())
      q.push({ ln, state, buffer });
}

bool LEXER::store(){
   if (empty()){ 
      while (!q.empty()){
         switch (q.front().state){ 
            case 1:
               isK(s) ? insert(q.front().ln, "KEYWORD", q.front().lex) : insert(q.front().ln, "IDENTIFIER", q.front().lex);
               break;
            case 2:
               insert(q.front().ln, "INTEGER", q.front().lex);
               break;
            case 3:
               insert(q.front().ln, "REAL", q.front().lex);
               break;
            case 4:
               insert(q.front().ln, "SEPARATOR", q.front().lex);
               break;
            case 5:
               insert(q.front().ln, "OPERATOR", q.front().lex);
               break;
            case 6: 
               insert(q.front().ln, "PUNCTUATION", q.front().lex);
               break;
            default:
               break;
         }
         q.pop();
      }
      insert(0, "OPERATOR", "%");
      return true;
   }
   else {
      display();
      return false;
   }
}

void LEXER::printToken(){
   system("cls");

   if (empty()){
      cout << "+------------------------------+\n" << "| TOKENS" << setw(24) << "LEXEMES |" << "\n+------------------------------+\n";
      while (!q.empty()){
         switch (q.front().state){ 
            case 1:
               if (isK(q.front().lex)){
                  cout << right << "| KEYWORD" << setw(8) << "=" << right << setw(13) << q.front().lex << " |" << endl; 
                  insert(q.front().ln, "KEYWORD", q.front().lex);
               }
               else {
                  cout << right << "| IDENTIFIER" << setw(5) << "=" << right << setw(13) << q.front().lex << " |" << endl;
                  insert(q.front().ln, "IDENTIFIER", q.front().lex); 
               }
               break;
            case 2:
               cout << right << "| INTEGER" << setw(8) << "=" << right << setw(13) << q.front().lex << " |" << endl;
               insert(q.front().ln, "INTEGER", q.front().lex);
               break;
            case 3:
               cout << right << "| REAL" << setw(11) << "=" << right << setw(13) << q.front().lex << " |" << endl;
               insert(q.front().ln, "REAL", q.front().lex);
               break;
            case 4:
               cout << right << "| SEPARATOR" << setw(6) << "=" << right << setw(13) << q.front().lex << " |" << endl;
               insert(q.front().ln, "SEPARATOR", q.front().lex);
               break;
            case 5:
               cout << right << "| OPERATOR" << setw(7) << "=" << right << setw(13) << q.front().lex << " |" << endl;
               insert(q.front().ln, "OPERATOR", q.front().lex);
               break;
            case 6: 
               cout << right << "| PUNCTUATION" << setw(4) << "=" << right << setw(13) << q.front().lex << " |" << endl;
               insert(q.front().ln, "PUNCTUATION", q.front().lex);
               break;
            default:
               break;
         }
         q.pop();
      }
      cout << "+------------------------------+\n";
   }
   else
      display();
}

void LEXER::reset(){
   q.push({ln, state, buffer});
   buffer = "";
}

symbol LEXER::request(){
   return getTable();
}
