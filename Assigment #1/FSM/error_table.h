#pragma once

#include <iostream>
#include <string>
#include <queue> // queue data structure

using namespace std;

class ErrorTable {
private:
   queue<string> error;
   size_t line_num = 0; // holds the current line number

public:
   void handler(const string& w); // will determine the error from a given string
   void inc();   // increments the line number by a factor of 1
   void print(); // prints all occuring errors
   bool empty(); // checks if queue is empty; thus, no error had occured
};

void ErrorTable::handler(const string& w){ // determines which type of error occured
   error.push("Invalid Token: \"" + w + "\" on Line: " + to_string(line_num) + "\n");
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){ // invalid integer
            string temp = " -> Expected Numeric Value at: \'";
            error.push(temp + W);
         }
      }
      if (w.find('.') != string::npos) // invalid floating data tye
         error.push(" -> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_') // invalid identifer of keyword
      error.push(" -> Identifiers cannot start with \'" + w);
   error.push("\'\n\n");
}

void ErrorTable::inc(){
   line_num++;
}

bool ErrorTable::empty(){
   error.empty() ? true : false; // returns if an error occured
}

void ErrorTable::print(){
   while (!empty()){
      cout << error.front();
      error.pop();
   }
}
