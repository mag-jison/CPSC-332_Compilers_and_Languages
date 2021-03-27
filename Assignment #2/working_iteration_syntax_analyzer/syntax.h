#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "tree.h"
#include "lexer.h"
#include "error_table.h"

using namespace std;

typedef queue<pair<string, string>> symbol;
//typedef Tree<string> parse;

class Syntax {
private:
//   parse tree;
   string backup;

public:
   void RDP(symbol);
   bool E(symbol&);
   bool Q(symbol&);
   bool T(symbol&);
   bool R(symbol&);
   bool F(symbol&);
   bool D(symbol&);
   bool A(symbol&);
   bool S(symbol&);
   bool O(symbol&);
   bool TY(symbol&);
   bool ID(symbol&);
   bool NUM(symbol&);
};

bool Syntax::NUM(symbol& w){
   return (w.front().first == "INTEGER" || w.front().first == "REAL") ? true : false;
}

bool Syntax::O(symbol& w){
   return (w.front().first == "OPERATOR") ? true : false;
}

bool Syntax::S(symbol& w){
   return (w.front().first == "SEPARATOR") ? true : false;
}

bool Syntax::ID(symbol& w){
   return (w.front().first == "IDENTIFIER") ? true : false;
}

bool Syntax::TY(symbol& w){
   return (w.front().first == "KEYWORD") ? true : false;
}

bool Syntax::E(symbol& w){
   cout << "<E> -> <T> <Q>" << endl;
   if (T(w)){
      if (Q(w)){
         return true;
      }
      else
         return false;
   }
   else
      return false;
}

bool Syntax::Q(symbol& w){
//   cout << "\tjmp -> <Q>" << endl;
   if (w.front().second == "+" || w.front().second == "-"){
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      cout << "<Q> -> " << w.front().second << " <T> <Q> | <e>" << endl;
      w.pop();
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      if (T(w)){
         if (Q(w)){
            return true;
         }
         else
            return false;
      }
      else
         return false;
   }
   else {
      cout << "<Q> -> <e>" << endl;
      return true;
   }
}

bool Syntax::T(symbol& w){
//   cout << "\tjmp -> <T>" << endl;
   cout << "<T> -> <F> <R>" << endl;
   if (F(w)){
      if (R(w)){
         return true;
      }
      else
         return false;
   }
   else
      return false;
}

bool Syntax::R(symbol& w){
//   cout << "\tjmp -> <R>" << endl;
   if (w.front().second == "*" || w.front().second == "/"){
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      cout << "<R> -> " << w.front().second << " <F> <R> | <e>" << endl;
      w.pop();
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      if (F(w)){
         if (R(w)){
            return true;
         }
         else
            return false;
      }
      else
         return false;
   }
   else {
      cout << "<R> -> <e>" << endl;
      return true;
   }
}

bool Syntax::F(symbol& w){
//   cout << "\tjmp -> <F>" << endl;
   if (w.front().second == "("){
      cout << "<F> -> ( <E>" << endl;
      w.pop();
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      if (E(w)){
         if (w.front().second == ")"){
            cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";  
            cout << "<F> -> ( <E> )" << endl;
            w.pop();
            return true;
         }
         else
            return false;
      }
      else
         return false;
   }
   else if (ID(w)){
      cout << "<F> -> <ID>\n<ID> -> " << w.front().second << endl;
      w.pop();
      return true;
   }
   else if (NUM(w)){
      cout << "<F> -> <NUM>\n<NUM> -> " << w.front().second << endl;
      w.pop();
      return true;
   }
   else
      return false;
}

bool Syntax::D(symbol& w){
   if (TY(w)){
      cout << "<S> -> <D>\n<D> -> <TY> <ID>\n";
      cout << "<TY> -> " << w.front().second << endl;
      w.pop();
      cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
      if (ID(w)){
         cout << "<ID> -> " << w.front().second << endl;
         w.pop();
         return true;
      }
      else
         return false; 
   }
   else
      return false;
}

bool Syntax::A(symbol& w){
//   cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n\n";
   if (ID(w)){
      cout << "<E> -> <A>\n<A> -> <ID> = <E>" << endl;
      cout << "<ID> -> " << w.front().second << endl;
      w.pop();
      if (O(w)){
         cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
         w.pop();
         cout << "\nToken: " << w.front().first << "           Lexeme: " << w.front().second << "\n----------------\n";
         if (E(w))
            return true;
         else
            return false;
      }
      else
         return false;
   }
   else
      return false;
}

void Syntax::RDP(symbol table){
   system("cls");
   while (!table.empty()){
      cout << "\n\nToken: " << table.front().first << "           Lexeme: " << table.front().second << "\n----------------\n";
      if (D(table) || A(table) || E(table))
         cout << "\033[32m" << "\nSyntactically Correct!" << "\033[0m";
      else
         cout << "\033[91m" << "\nSyntactically Incorrect!" << "\033[0m";
   }
}
