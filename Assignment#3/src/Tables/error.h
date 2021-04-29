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
   void handler(const string&, const int&);
   void handler(const int&, const string&);
   void handler(const pair<size_t, string>&, const string&);
   void display();
   bool empty();
};

void ErrorTable::handler(const int& ln, const string& w){
   error.push("\033[91m");
   error.push("\nInvalid Token: ");
   error.push("\033[0m");
   error.push("\"" + w + "\" on Line: " + to_string(ln) + "\n");
   
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){
            string temp = "\t-> Expected Numeric Value at: \'";
            error.push(temp + W);
         }
      }
      if (w.find('.') != string::npos)
         error.push("\t-> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_')
      error.push("\t-> Identifiers cannot start with \'" + w);
   error.push("\'\n");
}

void ErrorTable::handler(const pair<size_t, string>& p, const string& q){
      error.push("\033[91m");
      if (q == "<E>")
         error.push("INVALID EXPRESSION");
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

      error.push(" on line: " + to_string(p.first) + "\n\texpected -> ");
      error.push("\'" + q + "\' after lexeme \'" + p.second + "\'\n");
      display();
}

void ErrorTable::handler(const string& t, const int& i){
   switch (i) {
      case 1:
         error.push("\033[91m");
         error.push("INVALID DECLARATION");
         error.push("\033[0m");
         error.push("\n\t-> \'");
         error.push(t);
         error.push("\' has already been declared previously in the file\n");
         break;
      case 2:
         error.push("\033[91m");
         error.push("UNDEFINED TOKEN");
         error.push("\033[0m");
         error.push("\n\t-> no reference to \'");
         error.push(t);
         error.push("\' can be found\n");
         break;
      default:
         break;;
   }
   display();
}

bool ErrorTable::empty(){
   return error.empty() ? true : false;
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
