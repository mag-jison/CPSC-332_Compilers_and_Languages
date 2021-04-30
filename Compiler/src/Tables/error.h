#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <utility>

using namespace std;

class ErrorTable {
private:
   queue<string> e;

public:
   void handler(const string&, const size_t&, const size_t&);
   void handler(const int&, const string&);
   void handler(const pair<size_t, string>&, const string&);
   void display();
   bool empty();
};

void ErrorTable::handler(const int& ln, const string& w){
   string temp;
   temp += w[0];

   e.push("\033[91m");
   e.push("Invalid Token: ");
   e.push("\033[0m");
   e.push("\"" + w + "\" on Line: " + to_string(ln) + "\n");
   
   if (isdigit(w[0])){
      for (char W : w){
         if (!isdigit(W) && W != '.'){
            string temp = "\t-> Expected Numeric Value at: \'";
            e.push(temp + W);
         }
      }
      if (w.find('.') != string::npos)
         e.push("\t-> Invalid Float Data Type at: \'.");
   }
   else if (w[0] == '$' || w[0] == '_')
      e.push("\t-> Identifiers cannot start with \'" + temp);
   e.push("\'\n\n");
}

void ErrorTable::handler(const pair<size_t, string>& p, const string& q){
      e.push("\033[91m");
      if (q == "<E>")
         e.push("INVALID EXPRESSION");
      else if (q == "<ID>")
         e.push("INVALID IDENTIFIER");
      else if (q == ")")
         e.push("INVALID SEPARATOR");
      else if (q == "<S>")
         e.push("INVALID SENTENCE");
      else if (q == ")")
         e.push("INVALID SEPARATOR");
      else if (q == "<C>")
         e.push("INVALID CONDITIONAL");
      else
         e.push("INVALID TOKEN");
      e.push("\033[0m");

      e.push(" on line: " + to_string(p.first) + "\n\t-> expected ");
      e.push("\'" + q + "\' after lexeme \'" + p.second + "\'\n\n");
      display();
}

void ErrorTable::handler(const string& t, const size_t& r, const size_t& i){
   switch (i) {
      case 1:
         e.push("\033[91m");
         e.push("INVALID DECLARATION");
         e.push("\033[0m");
         e.push("\n\t-> \'");
         e.push(t);
         e.push("\' has already been declared previously in the file on line: ");
         e.push(to_string(r));
         e.push("\n");
         for (int i = 0; i < 12; ++i)
            e.push(" ");
         e.push("^");
         break;
      case 2:
         e.push("\033[91m");
         e.push("UNDEFINED TOKEN");
         e.push("\033[0m");
         e.push("\n\t-> no reference to \'");
         e.push(t);
         e.push("\' can be found on line: ");
         e.push(to_string(r));
         e.push("\n");
         for (int i = 0; i < 28; ++i)
            e.push(" ");
         e.push("^");
         break;
      default:
         break;
   }

   for (int i = 0; i < t.length()-1; ++i)
      e.push("~");
      
   e.push("\n\n");
}

void ErrorTable::display(){
   while (!empty()){
      cout << e.front();
      e.pop();
   }
}

bool ErrorTable::empty(){
   return e.empty() ? true : false;
}