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

typedef queue<Token> symbol;
typedef vector<vector<size_t>> transition;

// holds all data for tokens
struct T { 
   size_t ln;
   size_t state;
   string lexeme;
};

class Lexer {
private:
   ErrorTable e;             // creates an ErrorTable object
   SymbolTable sym;          // creates a SymbolTable object
   size_t state, col, ln;    // holds the state, current column and line number
   string buffer, s, o;      // buffer holds the current lexeme
   vector<string> k;         // temporary structure to hold all valid keywords
   queue<T> q;               // temporary stucture to hold all meaningful units
   
//                      A  #  F  $  !  S  O  D  P
   transition table  {{ 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // start 
                      { 1, 1, 8, 1, 7, 4, 5, 0, 6 },  // alphabet 
                      { 8, 2, 3, 8, 7, 4, 5, 0, 6 },  // numeric
                      { 8, 3, 8, 8, 7, 4, 5, 0, 6 },  // real
                      { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // separator 
                      { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // operator
                      { 1, 2, 3, 8, 7, 4, 5, 0, 6 },  // punctuation
                      { 7, 7, 7, 7, 0, 7, 7, 7, 7 },  // comment
                      { 0, 0, 0, 0, 0, 0, 0, 0, 0 }}; // invalid token 
public:
   Lexer();                  // Default Constructor
   ~Lexer();                 // Destructor
   void FSM(const string&);  // parses each token and determines if the string w is part of the language
   int getCol(const char&);  // returns a specific column number from tState
   int getSign(const char&); // returns a unique integer referring to a particular sign
   bool isD(const char&);    // checks if input is a delimiter
   bool isK(const string&);  // checks if string input is a keyword
   bool wasSOP();            // checks if current state is of a separator or operator
   void reset();             // pushes a valid token and empties the buffer
   bool insert();            // stores every token, lexeme, and line number in the symbol table without printing
   void display();           // stores every token, lexeme, and line number in the symbol table and prints only the tokens and lexemes
   void viewTable();         // helper function to get symboltable from main function
   symbol request();         // function that returns the symbol table for the parser
};

Lexer::Lexer(){
   this->ln = 0;
   this->k = {"int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};
   this->s = "(){}[],.:;";
   this->o = "*+-=/><%";
}

Lexer::~Lexer(){
   for (vector<size_t> element : table)
      element.clear(); // empties the transition table
}


bool Lexer::isK(const string& w){
   for (string K : k){
      if (K == w){
         q.front().state--; // keyword has a state of 0
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
         q.push({ln, state, buffer});
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
   ln++;                            // increases for every new line of input

   for (size_t i = 0; i < w.length(); ++i){  // reads char by char
      col = getCol(w[i]);                    // gets the column with respect to the char
      state = table[state][col];            // jumps to a new or current state
      buffer += w[i];                        // adds the lexeme to the buffer

      if (isD(w[i]))
         buffer = "";     // resets after a delimiter
      else if (wasSOP())  // prints the separator, operator or punctuation
         reset();
      else if (state == 8)
         e.handler(ln, buffer); // records the error which ended in a dead state 
   }

   if (!wasSOP())
      q.push({ln, state, buffer}); // pushes into the queue if current state is not a separator nor operator
}

bool Lexer::insert(){
   int f;
   string s;

   if (e.empty()){ 
      while (!q.empty()){
         f = q.front().state;
         s = q.front().lexeme;

         switch (f){ 
            case 1:
               isK(s) ? sym.insert(q.front().ln, "KEYWORD", q.front().lexeme) : sym.insert(q.front().ln, "IDENTIFIER", q.front().lexeme);  // saves tokens and lexemes for future reference
               break;
            case 2:
               sym.insert(q.front().ln, "INTEGER", q.front().lexeme); // saves tokens and lexemes for future reference
               break;
            case 3:
               sym.insert(q.front().ln, "REAL", q.front().lexeme); // saves tokens and lexemes for future reference
               break;
            case 4:
               sym.insert(q.front().ln, "SEPARATOR", q.front().lexeme); // saves tokens and lexemes for future reference
               break;
            case 5:
               sym.insert(q.front().ln, "OPERATOR", q.front().lexeme); // saves tokens and lexemes for future reference
               break;
            case 6: 
               sym.insert(q.front().ln, "PUNCTUATION", q.front().lexeme); // saves tokens and lexemes for future reference
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

   if (e.empty()){ // will only execute if no error exists; state 8 has never been reached
      string line, border = "+------------------------------+\n";
      line.assign(28, '-'); // initializes 'line' with a multiple of 28 '-' characters
      cout << border << "| TOKENS" << setw(25) << "LEXEMES |\n" << border; // console prompt

      while (!q.empty()){
         switch (q.front().state){ 
            case 1:
               if (isK(q.front().lexeme)){
                  cout << right << "| KEYWORD" << setw(8) << "=" << right << setw(13) << q.front().lexeme << " |" << endl; 
                  sym.insert(q.front().ln, "KEYWORD", q.front().lexeme);
               }
               else {
                  cout << right << "| IDENTIFIER" << setw(5) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
                  sym.insert(q.front().ln, "IDENTIFIER", q.front().lexeme); 
               }
               break;
            case 2:
               cout << right << "| INTEGER" << setw(8) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
               sym.insert(q.front().ln, "INTEGER", q.front().lexeme);
               break;
            case 3:
               cout << right << "| REAL" << setw(11) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
               sym.insert(q.front().ln, "REAL", q.front().lexeme);
               break;
            case 4:
               cout << right << "| SEPARATOR" << setw(6) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
               sym.insert(q.front().ln, "SEPARATOR", q.front().lexeme);
               break;
            case 5:
               cout << right << "| OPERATOR" << setw(7) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
               sym.insert(q.front().ln, "OPERATOR", q.front().lexeme);
               break;
            case 6: 
               cout << right << "| PUNCTUATION" << setw(4) << "=" << right << setw(13) << q.front().lexeme << " |" << endl;
               sym.insert(q.front().ln, "PUNCTUATION", q.front().lexeme);
               break;
            default:
               break;
         }
         q.pop();
      }
      cout << border; // formats output
   }
   else
      e.display(); // prints all the errors that occured during scanning phase
}

void Lexer::reset(){ // resets the buffer
   q.push({ln, state, buffer});
   buffer = "";
}

symbol Lexer::request(){
   return sym.getTable();
}

void Lexer::viewTable(){
   sym.prompt();
}

