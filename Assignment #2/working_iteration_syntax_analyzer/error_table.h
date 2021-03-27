#pragma once

#include <iostream>
#include <string>
#include <queue>

using namespace std;

class ErrorTable {
private:
   queue<string> error;
   size_t line_num = 0;

public:
   void handler(const string& w);
   void inc();  
   void display();
   bool empty();
};

void ErrorTable::handler(const string& w){
   cout << "\033[91m" << "Invalid Token: " << "\033[0m";
   error.push("\"" + w + "\" on Line: " + to_string(line_num) + "\n");
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){
            string temp = " -> Expected Numeric Value at: \'";
            error.push(temp + W);
         }
      }
      if (w.find('.') != string::npos)
         error.push(" -> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_')
      error.push(" -> Identifiers cannot start with \'" + w);
   error.push("\'\n\n");
}

void ErrorTable::inc(){
   line_num++;
}

bool ErrorTable::empty(){
   return error.empty() ? true : false;
}

void ErrorTable::display(){
   while (!empty()){
      cout << error.front();
      error.pop();
   }
}
