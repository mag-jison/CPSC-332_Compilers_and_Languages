/*
 * Working Interation of Syntax Analyzer
 * By: Jason Duong
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>

#include "lexer.h"
#include "tree.h"
#include "symbol_table.h"
#include "error_table.h"

using namespace std;

typedef queue<pair<string, string>> symbol;

class Syntax {
private:
   Tree<pair<string, string>> statement;

public:
   Syntax(){
      statement.insert({"7<S>", ""});
      statement.insert({"1<A>", ""});
      statement.insert({"8<D>", ""});
      statement.insert({"7<T>", ""});
      statement.insert({"9<ID>", ""});
      statement.insert({"0<ID>", ""});
      statement.insert({"3<E>", ""});
      statement.insert({"2<ID>", ""});
      statement.insert({"4<ID>", ""});
   }
   
   void parse(symbol);
};

void Syntax::parse(symbol table){
   system("cls");
   string f, s;

   while (!table.empty()){
      f = table.front().first;
      s = table.front().second;
      if (f == "IDENTIFIER"){
         cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
         statement.replace({"0<ID>", s});
         table.pop();
         f = table.front().first;
         s = table.front().second;
         if (f == "OPERATOR"){
            cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
            statement.replace({"1<A>", s});
            table.pop();
            f = table.front().first;
            s = table.front().second;
            if (f == "IDENTIFIER"){
               cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
               statement.replace({"2<ID>", s});
               table.pop();
               f = table.front().first;
               s = table.front().second;
               if (f == "OPERATOR"){
                  cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
                  statement.replace({"3<E>", s});
                  table.pop();
                  f = table.front().first;
                  s = table.front().second;
                  if (f == "IDENTIFIER"){
                     cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
                     statement.replace({"4<ID>", s});
                     table.pop();
                     f = table.front().first;
                     s = table.front().second;
                     if (f == "SEPARATOR"){
                        cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
                        table.pop();
                      }
                  }
                  else {
                     cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
                     exit(0);
                  }
               }
               else {
                  cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
                  exit(0);
               }
            }
            else {
               cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
               exit(0);
            }
         }
         else {
            cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
            exit(0);
         }
      }
      else if (f == "KEYWORD"){
         cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
         statement.replace({"7<T>", s});
         table.pop();
         f = table.front().first;
         s = table.front().second;
         if (f == "IDENTIFIER"){
            cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
            statement.replace({"9<ID>", s});
            table.pop();
            f = table.front().first;
            s = table.front().second;
            if (f == "SEPARATOR"){
               cout << "Token: " << f << setw(16-f.length()) << "|" << "    Lexeme: " << s << endl;
               table.pop();
            }
         }
         else {
            cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
         }
      }
      else {
         cout << "INVALID TERMINAL FOR STRUCTURE: " << s << endl;
         exit(0);
      }
      statement.preOrder();
      statement.printTree();
   }
}
