#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "lexer.h"
#include "error_table.h"

using namespace std;

#define token w.front().first
#define lexeme w.front().second
#define write output << "\nToken: " << token << "\nLexeme: " << lexeme << "\n----------------\n"
typedef queue<pair<string, string>> symbol;

class Syntax {
private:
   ofstream output;
   string g;

public:
   void RDP(symbol);
   void terminate();
   bool root(symbol&);
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
   return (token == "INTEGER" || token == "REAL") ? true : false;
}

bool Syntax::O(symbol& w){
   return (token == "OPERATOR") ? true : false;
}

bool Syntax::S(symbol& w){
   return (token == "SEPARATOR") ? true : false;
}

bool Syntax::ID(symbol& w){
   return (token == "IDENTIFIER") ? true : false;
}

bool Syntax::TY(symbol& w){
   return (token == "KEYWORD") ? true : false;
}

bool Syntax::root(symbol& w){
   return (A(w) || D(w) || E(w)) ? true : false;
}

bool Syntax::E(symbol& w){
   output << "\t<E> -> <T> <Q>" << endl;
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
   output << "\tjmp -> <Q>" << endl;
   if (lexeme == "+" || lexeme == "-"){
      write;
       output << "\t<Q> -> " << lexeme << " <T> <Q> | epsilon" << endl;
      g = g + lexeme + " ";
      w.pop();
      write;
      if (T(w)){
         if (Q(w))
            return true;
         else
           return false; 
      }
      else
        return false; 
   }
   else {
       output << "\t<Q> -> epsilon" << endl;
      return true;
   }
}

bool Syntax::T(symbol& w){
   output << "\tjmp -> <T>" << endl;
    output << "\t<T> -> <F> <R>" << endl;
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
   output << "\tjmp -> <R>" << endl;
   if (lexeme == "*" || lexeme == "/"){
      write;
       output << "\t<R> -> " << lexeme << " <F> <R> | e" << endl;
      g = g + lexeme + " ";
      w.pop();
      write;
      if (F(w)){
         if (R(w))
            return true;
         else
           return false;
      }
      else
        return false;
   }
   else {
       output << "\t<R> -> epsilon" << endl;
      return true;
   }
}

bool Syntax::F(symbol& w){
   output << "\tjmp -> <F>" << endl;
   if (lexeme == "("){
       output << "\t<F> -> ( <E>" << endl;
      g = g + lexeme + " ";
      w.pop();
      write;
      if (E(w)){
         if (lexeme == ")"){
            write;
             output << "\t<F> -> ( <E> )" << endl;
            g = g + lexeme + " ";
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
       output << "\t<F> -> <ID>\n\t<ID> -> " << lexeme << endl;
      g = g + lexeme + " ";
      w.pop();
      return true;
   }
   else if (NUM(w)){
       output << "\t<F> -> <NUM>\n\t<NUM> -> " << lexeme << endl;
      g = g + lexeme + " ";
      w.pop();
      return true;
   }
   else
     return false; 
}

bool Syntax::D(symbol& w){
   if (TY(w)){
       output << "\t<S> -> <D>\n\t<D> -> <TY> <ID>\n\t<TY> -> " << lexeme << endl;
      g = g + lexeme + " ";
      w.pop();
      write;
      if (ID(w)){
          output << "\t<ID> -> " << lexeme << endl;
         g = g + lexeme + " ";
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
   if (ID(w)){
       output << "\t<S> -> <A>\n\t<A> -> <ID> = <E>\n\t<ID> -> " << lexeme << endl;
      g = g + lexeme + " ";
      w.pop();
      if (lexeme == "="){
         write;
         g = g + lexeme + " ";
         w.pop();
         write;
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
   output.open("out.txt");
   
   while (!table.empty()){
      g = "";
      output << "Token: " << table.front().first << "\nLexeme: " << table.front().second << "\n----------------\n";
      if (root(table))
         output << "\n\t***ACCEPTED***" << " => " << g << "\n\n";
      else {
         output << "\n\t***NOT ACCEPTED***" << " => " << g << "___";
         terminate(); 
      }
   }
   cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n";
   output.close();
}

void Syntax::terminate(){
   cout << "\033[91m" << "FAILED: " << "\033[0m" << "view in out.txt\n";
   output.close();
   system("pause");
   exit(0);
}
