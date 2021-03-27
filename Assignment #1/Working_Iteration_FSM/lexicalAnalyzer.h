#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cctype> //isalpha()
#include <iomanip> //setw()
#include <map>

using namespace std;

enum Token {keyword, separator, identifier, integer, reals, operators, errors} type; //list of Token data types

class lexicalAnalyzer {
private:
   string buffer;
public:
   lexicalAnalyzer(){
      this->buffer = "";
   }

   Token lexer(const string&);
   void print(const Token&);
};

Token lexicalAnalyzer::lexer(const string& x){ //parameter x can only be read
      fstream k("keywords.txt"), s("separators.txt"), o("operators.txt"); //opens the files for checking

      int i = 0; //current character position
      string key; //key is the word being read from file
      buffer += x[i]; //initializes buffer to first character in word

      while (o >> key){
         if (x.find(key) != string::npos){
            buffer = x;
            return operators; //operators
         }
      }

      while (s >> key){
         if (x.find(key) != string::npos && !isdigit(x[i])){
            buffer = x;
            return separator; //separator
         }
      }

      if (isdigit(x[i])){ //checks if char is digit
         while (isdigit(x[++i]) || x[i] == '.') //checks for preceding numbers
            buffer += x[i];
         if (buffer.find('.') != string::npos)
            return reals; //reals
         return integer; //integers
      }

      else if (x[i] == '!'){
         while (x[++i] != '!')
            buffer += x[i];
         buffer += '!';
         return errors; //comments      
      }

      else if (isalpha(x[i])){ //checks if character is an alphabet
         while (isalpha(x[++i]) || isdigit(x[i]) || x[i] == '_' || x[i] == '$') //checks if preceding character is an alphabet
            buffer += x[i];
         while (k >> key){
            if (buffer.compare(key) == 0) 
               return keyword; //keywords
         }
         return identifier; //identifiers
      }

      else
         buffer = "Invalid Token"; //case when token is unrecognizable to the language
   k.close(), s.close(), o.close();
}

void lexicalAnalyzer::print(const Token& type){
   switch (type){
      case keyword:
         cout << right << "KEYWORD" << setw(8) << "=" << left << right << setw(15) << buffer << endl; //user output
         break;
      case separator:
         cout << right << "SEPARATOR" << setw(6) << "=" << left << right << setw(15) << buffer << endl;
         break;
      case identifier:
         cout << right << "IDENTIFIER" << setw(5) << "=" << left << right << setw(15) << buffer << endl;
         break;
      case integer:
         cout << right << "INTEGER" << setw(8) << "=" << left << right << setw(15) << buffer << endl; 
         break;
      case reals:
         cout << right << "REAL" << setw(11) << "=" << left << right << setw(15) << buffer << endl;
         break;
      case operators:
         cout << right << "OPERATOR" << setw(7) << "=" << left << right << setw(15) << buffer << endl;
         break;
      case errors:
         break;
   }
   buffer = ""; //empties the buffer
}

   
