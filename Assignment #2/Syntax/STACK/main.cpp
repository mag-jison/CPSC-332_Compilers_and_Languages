/*
 * !!! DO NOT GRADE THIS PARSER !!!
*/

#include <iostream>
#include <string>
#include <fstream>

#include "../../Lexer/lexer.h"
#include "Stack.h"

using namespace std;

int main(){
   fstream myfile;
   string ui;
   char c;
   int x;

   Lexer l;
   Stack s;

   system("cls"); // clears the screen
   cout << "Enter a Test File-Name: test/"; 
   cin >> ui;
   myfile.open("test/" + ui);

   if (myfile.is_open()){
      while (getline(myfile, ui))
         l.FSM(ui);
      l.insert();
      s.STACK(l.request());
   }
   else
      cout << "No Files Found!\n"; 

   system("pause>nul");
   return 0;
}
