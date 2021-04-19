#include <iostream> // cout & cin
#include <string>
#include <fstream>
#include <conio.h> // _getch()

#include "lexer.h"
#include "syntax.h"

using namespace std;

int main(){
   fstream myfile;
   string ui;
   char c;
   bool on = true;
   int x;

   do {
      Lexer l;
      Syntax s;

      system("cls"); // clears the screen
      cout << "Enter a Test File-Name: test/"; 
      cin >> ui;
      myfile.open("test/" + ui);

      if (myfile.is_open()){ 

         while (getline(myfile, ui))
            l.FSM(ui);

         do {
            system("cls");
            on = !on;
            if (on)
               cout << "(0) View Productions: " << "\033[32m" << "ON" << "\033[0m\n\n";
            else
               cout << "(0) View Productions: " << "\033[91m" << "OFF" << "\033[0m\n\n";
      
            cout << "+----------------------+\n";
            cout << "|     C++ Compiler     |\n";
            cout << "+----------------------+\n";
            cout << "| (1) Lexical Analyzer |\n";
            cout << "| (2) Syntax Analyzer  |\n";
            cout << "| (3) Symbol Table     |\n";
            cout << "+----------------------+\n\n>> ";
            x = _getch();
         } while (x == 48);

         switch (x-48){ // prompts menu
            case 1:
                  l.display();
               break;
            case 2:
               if (l.insert())
                  s.RDP(l.request(), on); // request for tokens; will be used by parser
               break;
            case 3:
               if (l.insert())
                  l.viewTable(); // depository of all tokens
               break;
            default:
               system("cls");
               cout << "Invalid Input!\n"; // invalid input if user input was not 1, 2, or 3
               break;
         }
      }
      else
         cout << "No Files Found!\n"; 
      cout << "\nEnter new test file? (y/n) "; // allows for additional file processing
      cin >> c;
      myfile.close();
   } while (c == 'y');

   return 0;
}
