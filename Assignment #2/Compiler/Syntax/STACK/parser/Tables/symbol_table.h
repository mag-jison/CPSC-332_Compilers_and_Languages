#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iomanip>

using namespace std;

struct Token { // stores all information for every token
   size_t ln;
   string tok;
   string lex;
};

typedef queue<Token> symbol; // for ease in debugging code

class SymbolTable {
private:
   symbol table;
   string t, r, q; // used to query table for ease of finding relevant information

public:
   symbol getTable(); // returns a symbol table
   void insert(const size_t&, const string&, const string&); // inserts into a queue<Token> data structure
   void prompt();        // initial interface to give users options on what tasks for program to perform
   void print(symbol&);  // prints all tokens, lexemes and line numbers generated from the lexical analyzer
   void select(symbol);  // returns a table of all occurences of an input token, lexeme, and/or line number
   void project(symbol); // returns a table of all input attributes
};

void SymbolTable::prompt(){
   char c;
   do {
      system("cls");
      int x;
      cout << "+----------------------+\n";
      cout << "|     Symbol Table     |\n";
      cout << "+----------------------+\n";
      cout << "| (1) SELECT           |\n";
      cout << "| (2) PROJECT          |\n";
      cout << "| (3) HELP             |\n";
      cout << "+----------------------+\n\n>> ";
      cin >> x;
      switch (x){
         case 1:
            select(table);
            break;
         case 2:
            project(table);
            break;
         case 3:
            system("cls");
            cout << "SELECT: returns a table of all occurences of an input token, lexeme, and/or line number\n";
            cout << "PROJECT: returns a table of all input attributes\n";
            break;
         default:
            system("cls");
            cout << "Invalid Input!\n";
            break;
      }
      cout << "\nGo Back? (y/n) "; // allows users to re-input into program after selection of an input
      cin >> c;
   } while (c == 'y');
}

symbol SymbolTable::getTable(){
   return table;
}

void SymbolTable::insert(const size_t& ln, const string& token, const string& lexeme){
   table.push({ln, token, lexeme});
}

void SymbolTable::print(symbol& temp){ // prints everything in struct Token
   cout << "| " << temp.front().tok << setw(13-temp.front().tok.length()) << " | " << temp.front().lex << setw(14-temp.front().lex.length()) << " | " << temp.front().ln << setw(9-(to_string(temp.front().ln)).length()) << "|" << endl;
}

void SymbolTable::project(symbol temp){
   system("cls");
   cout << "PROJECT <input>\n\n<input>  -> <token> <lexeme> <#>\n<token>  -> y | n\n<lexeme> -> y | n\n<#>      -> y | n\n\nPROJECT ";
   cin >> t >> r >> q;

   system("cls");
   if (t == "y"){
      if (r == "y"){
         if (q == "y"){ // queries all columns
            cout << "+------------------------------------+\n";
            cout << "|   TOKENS   |   LEXEMES   |    #    |\n";
            cout << "|------------|-------------|---------|\n";
            while (!temp.empty()){
               print(temp);
               temp.pop();
            }
            cout << "+------------------------------------+";
         }
         else { // queries only tokens and lexemes
            cout << "+---------------------------+\n";
            cout << "|   TOKENS   |    LEXEMES   |\n";
            cout << "|------------|--------------|\n";
            while (!temp.empty()){
               cout << "| " << temp.front().tok << setw(13-temp.front().tok.length()) << " | " << temp.front().lex << setw(14-temp.front().lex.length()) << " |" << endl;
               temp.pop();
            }
            cout << "-----------------------------";
         }
      }
      else { // queries only tokens and line numbers
         if (q == "y"){
            cout << "+----------------------+\n";
            cout << "|   TOKENS   |    #    |\n";
            cout << "|------------|---------|\n";
            while (!temp.empty()){
               cout << "| " << temp.front().tok << setw(13-temp.front().tok.length()) << " | " << temp.front().ln << setw(9-(to_string(temp.front().ln)).length()) << " |" << endl;
               temp.pop();
            }
            cout << "----------";
         }
         else { // queries only tokens
            cout << "+------------+\n";
            cout << "|   TOKENS   |\n";
            cout << "|------------|\n";
         }
         while (!temp.empty()){
            cout << "| " << temp.front().tok << setw(12-temp.front().tok.length()) << " |" << endl;
            temp.pop();
         }
         cout << "--------------";
      }
   }
   else {
      if (r == "y"){
         if (q == "y"){ // queries only lexemes and line numbers
            cout << "+----------------------+\n";
            cout << "|  LEXEMES   |    #    |\n";
            cout << "|------------|---------|\n";
            while (!temp.empty()){
               cout << "| " << temp.front().lex << setw(13-temp.front().lex.length()) << " | " << temp.front().ln << setw(9-(to_string(temp.front().ln)).length()) << " |" << endl;
               temp.pop();
            }
            cout << "------------------------";
         }
         else { // queries only lexemes
            cout << "+-------------+\n";
            cout << "|   LEXEMES   |\n";
            cout << "|-------------|\n";
            while (!temp.empty()){
               cout << "| " << temp.front().lex << setw(13-temp.front().lex.length()) << " |" << endl;
               temp.pop();
            }
            cout << "---------------";
         }
      }
      else { // queries only line numbers
         if (q == "y"){
            cout << "+-------+\n";
            cout << "|   #   |\n";
            cout << "|-------|\n";
            while (!temp.empty()){
               cout << "| " << temp.front().ln << setw(7-(to_string(temp.front().ln)).length()) << " |" << endl;
               temp.pop();
            }
            cout << "---------";
         }
         else
            cout << "EMPTY SET"; // no data can be found by user query
      }
   }
}

void SymbolTable::select(symbol temp){
   system("cls");
   cout << "SELECT <input>\n\n<input>  -> <token> <lexeme> <line>\n<token>  -> token_name | *\n<lexeme> -> lexeme_name | *\n<line>   -> line_num | *\n*        -> all\n\nSELECT ";
   cin >> t >> r >> q;

   system("cls");
   cout << "+------------------------------------+\n";
   cout << "|   TOKENS   |   LEXEMES   |    #    |\n";
   cout << "|------------|-------------|---------|\n";
   if (t == "*"){
      if (r == "*"){
         if (q == "*"){
            while (!temp.empty()) { // user selects every token, lexeme and line number
               print(temp);
               temp.pop();
            }
         }
         else {
            while (!temp.empty()){ // user wants a specfic line number
               if (q == to_string(temp.front().ln))
                  print(temp);
               temp.pop();
            }
         }
      }
      else if (q == "*"){
         while (!temp.empty()){ // user wants a specific lexeme
            if (r == temp.front().lex)
               print(temp);
            temp.pop();
         }
      }
      else {
         while (!temp.empty()){ // users wants a specific lexeme and line number
            if (r == temp.front().lex && q == to_string(temp.front().ln))
               print(temp);
            temp.pop();
         }
      }
   }
   else if (r == "*"){
      if (q == "*"){
         while (!temp.empty()){ // user wants a specific token
            if (t == temp.front().tok)
               print(temp);
            temp.pop();
         }
      }
      else {
         while (!temp.empty()){ // user wants a specific token and line number
            if (t == temp.front().tok && q == to_string(temp.front().ln))
               print(temp);
            temp.pop();
         }
      }
   }
   else {
      if (q == "*") {
         while (!temp.empty()){ // user wants a specific token and lexeme
            if (t == temp.front().tok && r == temp.front().lex)
               print(temp);
            temp.pop();
         }
      }
      else {
         while (!temp.empty()){ // user wants a specific token, lexeme, and line number
            if (t == temp.front().tok && r == temp.front().lex && q == to_string(temp.front().ln))
               print(temp);
            temp.pop();
         }
      }
   }
   cout << "+------------------------------------+";   
}
