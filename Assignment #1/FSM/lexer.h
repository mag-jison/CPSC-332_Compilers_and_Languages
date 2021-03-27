#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>  // isalpha()
#include <iomanip> // setw()
#include <queue>
#include <utility> // pair structure

#include "symbol_table.h"
#include "error_table.h"

using namespace std;

class Lexer {
private:
   ErrorTable error;    // creates an ErrorTable object
   SymbolTable symbol;  // creates a SymbolTable object
   bool on = false;     // boolean switch
   size_t state, col;   // holds the row & column for the tState
   string buffer,       // buffer holds the current lexeme
          s = "(){}[],.:;",
          o = "*+-=/><%";

   vector<string> k {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};
   queue<pair<int, string>> q;   // temporary stucture to hold all meaningful units
   vector<vector<size_t>> tState 

//        A  #  F  $  !  S  O  D  P
       {{ 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // start 
        { 1, 1, 8, 1, 7, 4, 5, 0, 6 },  // alphabet 
        { 8, 2, 3, 8, 7, 4, 5, 0, 6 },  // numeric
        { 8, 3, 8, 8, 7, 4, 5, 0, 6 },  // real
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // separator 
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // operator
        { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // punctuation
        { 7, 7, 7, 7, 0, 7, 7, 7, 7 },  // comment
        { 0, 0, 0, 0, 0, 0, 0, 0, 0 }}; // invalid token 

public:
   void FSM(const string&);  // parses each token and determines if the string w is part of the language
   int getCol(const char&);  // returns a specific column number from tState
   int getSign(const char&); // returns a unique integer referring to a particular sign
   bool isD(const char&);    // checks if input is a delimiter
   bool isK(const string&);  // checks if string input is a keyword
   bool wasSOP();            // checks if current state is of a separator or operator
   void reset();             // pushes a valid token and empties the buffer
   void print();             // outputs every token and lexeme to the console
   void turnOn();            // activates the symbol table
};

void Lexer::turnOn(){
   on = true;
}

bool Lexer::isK(const string& w){
   for (string K : k){
      if (K == w){
         q.front().first--; // keyword has a state of 0
         return true;
      }
   }
   return false; // false = identifier
}

bool Lexer::isD(const char& c){
   return (c == ' ' || c == '\t') ? true : false; // does not check if new line because
}                                                 // input string is read line by line

bool Lexer::wasSOP(){
   return (state == 4 || state == 5 || state == 6) ? true : false;
}

int Lexer::getSign(const char& c){        // determines which sign and returns a unique number
   if (c == '_' || c == '$')  return 1;   // underscore & dollar signs
   if (c == '!')              return 2;   // comments

   for (char S : s){                      // separators
      if (S == c)
         return (S == '.') ? 0 : 5;       // if the separator is a period, it will go to another state than a separator
   }

   for (char O : o){                      // operators
      if (O == c)
         return 6;
   }
}

int Lexer::getCol(const char& c){
   int signs = getSign(c);
   if      (isalpha(c))    return 0;    // alphabets
   else if (isdigit(c))    return 1;    // numerics
   else if (signs == 0)    return 2;    // reals
   else if (signs == 1)    return 3;    // underscore & dollar signs
   else if (signs == 2)    return 4;    // comments
   else if (signs == 5 || signs == 6){  // separators & operators
      if (!wasSOP())                    // checks if preceding sign is a separator, operator or punctation
         reset();
      return signs;                     // the column number is one more than the return value
   }

   else if (isD(c)){                    // prints the string after a delimiter
      if (!wasSOP())
         q.push({state, buffer});
      return 7;
   }

   else {
      if (!wasSOP())
         reset();
      return 8; // defaults to a punctuation mark for unrecognize characters
   }
}

void Lexer::FSM(const string& w){
   buffer = "", state = 0, col = 0; // resets after every new line of input
   error.inc();                     // increases for every new line of input

   for (size_t i = 0; i < w.length(); ++i){  // reads char by char
      col = getCol(w[i]);                    // gets the column with respect to the char
      state = tState[state][col];            // jumps to a new or current state
      buffer += w[i];                        // adds the lexeme to the buffer

      if (isD(w[i]))
         buffer = "";     // resets after a delimiter
      else if (wasSOP())  // prints the separator, operator or punctuation
         reset();
      else if (state == 8)
         error.handler(buffer); // records the error which ended in a dead state 
   }

   if (!wasSOP())
      q.push({state, buffer}); // pushes into the queue if current state is not a separator nor operator
}

void Lexer::print(){
   system("cls");

   if (error.empty()){ // will only execute if no error exists; state 8 has never been reached
      string line, border = "+------------------------------+\n";
      line.assign(28, '-'); // initializes 'line' with a multiple of 28 '-' chars
      cout << border << "| TOKENS" << setw(25) << "LEXEMES |\n" << "| " << line << " |" << "\n"; // console prompt

      while (!q.empty()){
         switch (q.front().first){ // checks for current state
            case 1:
               if (isK(q.front().second))
                  cout << right << "| KEYWORD" << setw(8) << "=" << left << right << setw(13) << q.front().second << " |" << endl; // formats output
               else
                  cout << right << "| IDENTIFIER" << setw(5) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second); // saves tokens and lexemes for future reference
               break;
            case 2:
               cout << right << "| INTEGER" << setw(8) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second);
               break;
            case 3:
               cout << right << "| REAL" << setw(11) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second);
               break;
            case 4:
               cout << right << "| SEPARATOR" << setw(6) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second);
               break;
            case 5:
               cout << right << "| OPERATOR" << setw(7) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second);
               break;
            case 6: 
               cout << right << "| PUNCTUATION" << setw(4) << "=" << left << right << setw(13) << q.front().second << " |" << endl;
               symbol.insert(q.front().first, q.front().second);
               break;
            default:
               break;
         }
         q.pop();
      }
      cout << border; // formats output
      if (on)
         symbol.display(); // displays all lexemes of tokens
   }
   else
      error.print(); // prints all the errors that occured
}

void Lexer::reset(){ // resets the buffer
   q.push({state, buffer});
   buffer = "";
}
