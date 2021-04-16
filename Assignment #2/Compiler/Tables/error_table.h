#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <utility>

using namespace std;

class ErrorTable {
private:
   queue<string> error;

public:
   void handler(const int&, const string&); // handles lexical errors; errors due to the invalid tokens; the incorrect spelling of a word
   void handler(const pair<size_t, string>&, const string&); // handles syntatical errors; errors due to grammar and structure of the program; the incorrect ordering of a sentence
   void display(); // prints all occuring errors from a particular phase
   bool empty();   // checks if queue is empty; thus, no error had occured
};

void ErrorTable::handler(const int& ln, const string& w){
   error.push("\033[91m");
   error.push("\nInvalid Token: ");
   error.push("\033[0m");
   error.push("\"" + w + "\" on Line: " + to_string(ln) + "\n");
   
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){ // invalid integer
            string temp = " -> Expected Numeric Value at: \'";
            error.push(temp + W);
         }
      }
      if (w.find('.') != string::npos) // invalid floating data type
         error.push(" -> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_') // invalid identifier of keyword
      error.push(" -> Identifiers cannot start with \'" + w);
   error.push("\'\n");
}

void ErrorTable::handler(const pair<size_t, string>& p, const string& q){
      error.push("\033[91m");
      if (q == "<E>")
         error.push("INVALID EXPRESSION"); // error types
      else if (q == "<ID>")
         error.push("INVALID IDENTIFIER");
      else if (q == ")")
         error.push("INVALID SEPARATOR");
      else if (q == "<S>")
         error.push("INVALID SENTENCE");
      else if (q == ")")
         error.push("INVALID SEPARATOR");
      else if (q == "<C>")
         error.push("INVALID CONDITIONAL");
      else
         error.push("INVALID TOKEN");
      error.push("\033[0m");

      error.push(" on line: " + to_string(p.first) + "\nexpected => "); // gives location of error and possible solution
      error.push("\'" + q + "\' after lexeme \'" + p.second + "\'\n"); // gives indictor of where on the line the error had occured
      error.push("view in output.txt\n\n");
      display();
}

bool ErrorTable::empty(){
   return error.empty() ? true : false; // returns if an error occured
}

void ErrorTable::display(){
   system("cls");
   while (!empty()){
      cout << error.front();
      error.pop();
   }
   system("pause>nul");
   exit(0);
}
