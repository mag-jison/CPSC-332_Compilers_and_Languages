#include <iostream> // cout & cin
#include <string>
#include <fstream>

#include "lexer.h"

using namespace std;

int main(){
   Lexer obj; // creates a lexer object
   fstream myfile;
   string directory = "test/", ui; // ui stands for user input
   char c;

   system("cls"); // clears screen
   cout << "Enable Symbol Table? (y/n) ";
   cin >> c;

   system("cls");
   if (c == 'y'){
      obj.turnOn();
      cout << "Symbol Table: " << "\033[32m" << "ON" << "\033[0m" << endl;
   }
   else if (c == 'n')
      cout << "Symbol Table: " << "\033[91m" << "OFF" << "\033[0m" << endl;
   else {
      cout << "Invalid Choice\n";
      exit(0);
   }

   cout << "Enter a Test File-Name: test/"; // console prompt
   cin >> ui;
   directory += ui; // concatenates strings
   myfile.open(directory); // opens file
   system("cls");

   if (myfile.is_open()){ 
      while (getline(myfile, ui)) // reads line by line
         obj.FSM(ui); // calls function FSM
      obj.print();
   }
   else
      cout << "No Files Found!\n"; // checks if file exist

   myfile.close();
   system("pause>nul");
   return 0;
}
