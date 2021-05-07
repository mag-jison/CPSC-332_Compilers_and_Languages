#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include "LEXER.h"
#include "LR.h"

using namespace std;

int main(){
   fstream myfile;
   string file_name;
   char c;

   do {
      system("cls");
      myfile.open("1.txt");
      
      LEXER l(file_name);
      LR p(file_name);

      if (myfile.is_open()){ 
         while (getline(myfile, file_name))
            l.FSM(file_name);

         system("cls");
         if (l.store() && p.driver(l.request()))
            cout << "\033[32m" << "PASSED: " << "\033[0m" << "view in out.txt\n";
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
