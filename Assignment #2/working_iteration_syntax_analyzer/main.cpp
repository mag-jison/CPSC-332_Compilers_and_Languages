/*
  Production Rules for Declaration:
   <S> -> <D>
   <D> -> <TY> <ID>
   <TY> -> {bool, int, float}

  Production Rules for Assignment:
   <S> -> <A>
   <A> -> <ID> = <E>

  Production Rules for Expression with Left Factorization Applied:
   <S> -> <E>
   <E> -> <T> <Q>
   <Q> -> + <T> <Q> | <e>
   <T> -> <F> <R>
   <R> -> * <F> <R> | <e>
   <F> -> ( <E> ) | <ID>
   <ID> -> {o, i, a, b, c, d, x}
   

   Possible Improvements:
      ~ storing grammar in a parse tree
      ~ error table to enable syntactical errors: errors caused by the structure of the sequence of tokens
*/

#include <iostream> 
#include <string>
#include <fstream>

#include "lexer.h"
#include "syntax.h"

using namespace std;

int choice(){
   int x;
   cout << "+----------------------+\n";
   cout << "|     C++ Compiler     |\n";
   cout << "+----------------------+\n";
   cout << "| (1) Lexical Analyzer |\n";
   cout << "| (2) Syntax Analyzer  |\n";
   cout << "| (3) Symbol Table     |\n";
   cout << "| (4) Help             |\n";
   cout << "+----------------------+\n\n>> ";
   cin >> x;
   return x;
}

int main(){
   Lexer l;
   Syntax s;
   fstream myfile;
   string directory = "test/", ui;  

   system("cls");
   cout << "Enter a Test File-Name: test/"; 
   cin >> ui;
   directory += ui; 
   directory += ".txt";

   myfile.open(directory);
   system("cls");

   if (myfile.is_open()){ 
      while (getline(myfile, ui)) 
         l.FSM(ui); 
      switch(choice()){
         case 1:
            l.display();
            break;
         case 2:
            if (l.insert())
               s.RDP(l.request());
            break;
         case 3:
            if (l.insert())
               l.viewTable();
            break;
         case 4:
            system("cls");
            cout << "The Lexer Analyzer: parses all valid tokens and lexemes" << endl;
            cout << "The Syntax Analyzer: defines the structure of the language, grammar, using a set of rules, productions" << endl;
            cout << "The Symbol Table: stores relevant information on tokens" << endl;
            cout << "The Error Table: outputs all errors occured during compilation" << endl;
            break;
         default:
            system("cls");
            cout << "Invalid Input!\n";
            break;
      }
   }
   else
      cout << "No Files Found!\n"; 

   myfile.close();
   system("pause>nul");
   return 0;
}
