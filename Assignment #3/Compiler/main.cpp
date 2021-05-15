#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include "src/Lexer/LEXER.h"
#include "src/Syntax/PRDP.h"
#include "src/Syntax/STACK.h"
#include "src/Syntax/LR.h"
#define PASS "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n"

using namespace std;

bool on = false;

int prompt(const int& i){
   int x = -1;
   string w;

   system("cls");
   switch (i){
      case 1:
         cout << "+----------------------+\n";
         cout << "|     C++ Compiler     |\n";
         cout << "+----------------------+\n";
         cout << "| [1] Lexical Analyzer |\n";
         cout << "| [2] Syntax Analyzer  |\n";
         cout << "| [3] Symbol Table     |\n";
         cout << "| [4] Assembly Code    |\n";
         cout << "+----------------------+\n\n>> ";
         x = _getch();
         break;
      case 2:
         do {
            system("cls");
            on = !on;
            w = (on) ? "\033[32mON\033[0m" : "\033[91mOFF\033[0m";

            cout << "+---------------------+\n";
            cout << "|   Syntax Analyzer   |\n";
            cout << "+---------------------+\n";
            cout << "| [0] VIEW RULES: " << w << setw(15-w.length()) << "|\n";
            cout << "| [1] PRDP            |\n";
            cout << "| [2] STACK           |\n";
            cout << "! [3] LR              |\n";
            cout << "| [4] PARSE TREE      |\n";
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
   string file_name, w;
   char c;

   do {
      system("cls");
      cout << "Enter a Test File-Name: files/";
      cin >> w;
      file_name = "files/" + w;
      myfile.open(file_name);
      
      LEXER l(file_name);
      PRDP p(file_name);
      STACK s;
      LR r;

      if (myfile.is_open()){ 
         while (getline(myfile, w))
            l.FSM(w);

         switch (prompt(1)){
            case 1:
               l.printToken();
               break;
            case 2:
               system("cls");
               switch (prompt(2)) {
                  case 1:
                     if (l.store() && p.parser(l.request(), on))
                        cout << PASS;
                     break;
                  case 2:
                     if (l.store() && s.parser(l.request(), on))
                        cout << PASS;
                     break;
                  case 3:
                     if (l.store() && r.driver(l.request()))
                        cout << PASS;
                     break;
                  case 4:
                     if (l.store() && p.parser(l.request(), true))
                        cout << PASS;
                        p.getTree();
                     break;
                  default:
                     prompt(3);
               }
               break;
               case 3:
                  if (l.store() && p.parser(l.request(), true))
                     p.viewTable();
                  break;
               case 4:
                  if (l.store() && p.parser(l.request(), true))
                     p.printAL();
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
