#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>

using namespace std;

typedef queue<pair<string, string>> symbol;

class SymbolTable {
private:
   symbol table;

public:
   symbol getTable();
   void insert(const string&, const string&);
   void display();
};

symbol SymbolTable::getTable(){
   return table;
}

void SymbolTable::insert(const string& token, const string& lexeme){
   table.push({token, lexeme});
}

void SymbolTable::display(){
   system("cls");
   while (!table.empty()){
      cout << table.front().first << setw(13-table.front().first.length()) << " | " << table.front().second << endl;
      table.pop();
   }
}

