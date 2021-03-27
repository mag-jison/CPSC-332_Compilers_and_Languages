/*
 * Working Interation of Lexical Analyzer
 * By: Jason Duong
 * 
 * Note: only passes with spaces after each lexeme
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> //setw()

#include "lexicalAnalyzer.h"

using namespace std;

int main(){
   lexicalAnalyzer unit; //creates an object of a class
   fstream myfile; //creates a file object and opens it
   string file, input, line; //input is the word being read and file is the document being passed in.
   line.assign(30, '-'); //stores a repeated sequence of characters

   cout << "Enter a Test File Name: ";
   cin >> file; //prompts user
   myfile.open(file);
   system("cls");

   if (myfile.is_open()){ //test if file is opened
      cout << "TOKEN" << setw(26) << "LEXEMES\n" << line << "\n"; //user output    
      while (myfile >> input){ //reads file word by word
         type = unit.lexer(input); //sets the type (enum data type) from the lexer function
         unit.print(type);
      }
   }
   else
      cout << "No Files Found"; //outputs if no file was found

   myfile.close(); //closes file
   system("PAUSE");
   return 0;
}
