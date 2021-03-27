#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef vector<vector<string>> symbol;

class SymbolTable {
private:
   symbol table {{"KEYWORDS"}, {"IDENTIFIERS"}, {"INTEGERS"}, {"REALS"}, {"SEPARATORS"}, {"OPERATORS"}, {"PUNCTUATIONS"}}; // stores all valid lexemes in a vector for future reference

public:
   void insert(const int&, const string&);
   bool exist(const int&, const string&);
   void display();
};

bool SymbolTable::exist(const int& w, const string& s){
   for (size_t ow = 0; ow < table.at(w).size(); ++ow){
      if (s.compare(table[w][ow]) == 0)
         return true;
   }
   return false;
}

void SymbolTable::insert(const int& token, const string& lexeme){
   if (!exist(token, lexeme))
      table[token].push_back(lexeme);
}

void SymbolTable::display(){ // prints the adjacency matrix of Tokens & Lexemes
   system("cls");
   for (size_t i = 0; i < table.size(); ++i){
      if (table.at(i).size() != 1){
         cout << table[i][0] << ":\n";
         for (size_t j = 1; j < table.at(i).size(); ++j)
               cout << "\t" << table[i][j] << endl;
         cout << endl;
      }
   }
}
