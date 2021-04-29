#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include "src/Lexer/LEXER.h"
#include "src/Syntax/PRDP.h"
#include "src/Syntax/STACK.h"
#include "src/Assembly/ASSEMBLY.h"

using namespace std;

bool on = false;

int prompt(const int& i){
   int x = -1;

   system("cls");
   switch (i){
      case 1:
         cout << "+----------------------+\n";
         cout << "|     C++ Compiler     |\n";
         cout << "+----------------------+\n";
         cout << "| (1) Lexical Analyzer |\n";
         cout << "| (2) Syntax Analyzer  |\n";
         cout << "| (3) Symbol Table     |\n";
         cout << "| (4) Parse Tree       |\n";
         cout << "| (5) Assembly Code    |\n";
         cout << "+----------------------+\n\n>> ";
         x = _getch();
         break;
      case 2:
         do {
            system("cls");
            on = !on;
            cout << "(0) View Productions: ";
            if (on)
               cout << "\033[32m" << "ON" << "\033[0m\n\n";
            else
               cout << "\033[91m" << "OFF" << "\033[0m\n\n";

            cout << "+---------------------+\n";
            cout << "|   Syntax Analyzer   |\n";
            cout << "+---------------------+\n";
            cout << "| (1) PRDP            |\n";
            cout << "| (2) STACK           |\n";
            cout << "+---------------------+\n\n>> ";
            x = _getch();
         } while (x == 48);
         break;
      default:
         cout << "Invalid Input!\n";
         return x;
   }
   return x-48;
}

int main(){
   fstream myfile;
   string w;
   char c;

   do {
      LEXER l;
      PRDP p;
      STACK s;
      ASSEMBLY a;

      system("cls");
      cout << "Enter a Test File-Name: files/";
      cin >> w;
      myfile.open("files/" + w);


      if (myfile.is_open()){ 
         while (getline(myfile, w))
            l.FSM(w);

         switch (prompt(1)){
            case 1:
                  l.printToken();
               break;
            case 2:
               switch (prompt(2)) {
                  case 1:
                     if (l.store())
                        p.parser(l.request(), on);
                     break;
                  case 2:
                     if (l.store())
                        s.parser(l.request(), on);
                     break;
                  default:
                     prompt(3);
               }
               break;
            case 3:
               if (l.store()){
                  p.parser(l.request(), true);
                  p.viewTable();
               }
               break;
            case 4:
               if (l.store()){
                  s.parser(l.request(), true);
                  s.getTree();
               }
               break;
            case 5:
               system("cls");
               a.printAssembly();
               break;
            default:
               prompt(3);
         }
      }
      else {
         system("cls");
         cout << "No Files Found!\n";
      }

      myfile.close();
      cout << "\nEnter new test file? (y/n) ";
      cin >> c;

   } while (c == 'y');

   return 0;
}
