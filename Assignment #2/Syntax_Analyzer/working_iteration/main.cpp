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
   cout << "+----------------------+\n";
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
            l.display();
            s.parse(l.getTable());
            break;
         case 3:
            l.display();
            l.viewTable();
            break;
         default:
            system("cls");
            cout << "Invalid Input!";
            break;
      }
   }
   else
      cout << "No Files Found!\n"; 

   myfile.close();
   system("pause>nul");
   return 0;
}
