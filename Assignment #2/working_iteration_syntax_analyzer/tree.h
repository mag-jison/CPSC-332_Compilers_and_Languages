#pragma once

#include <iostream> 
#include <string>
#include <iomanip>
#include <queue>
#include <utility>

using namespace std;

// **************************************************************************************************
// NOTE BINARY TREE STRUCTURE IMPLEMENTATION FROM CPSC 131 REFERENCE TO HOMEWORK 8 EXCEPT printTree()
// NOTE FUNCTION: printTree()
//    AUTHOR: Adrian Schneider
//    LINK: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
// **************************************************************************************************

template <class T>
struct Node {
   T value;
   Node *left, *right;

   Node (T v) {
      value = v;
      left = nullptr;
      right = nullptr;
   }
};

template <class T>
class Tree {
private:
   Node<T>* root; 

public:
   Tree(){
      root = new Node<T>("E");
   }
   
   void find(string);
   bool findRecursive(Node<T>*, string);

   void insert(T);
   void insertRecursive(Node<T>*, Node<T>*, bool&);
   bool isTerminal(Node<T>*, Node<T>*);

   void printTree();
   void printTree(const string&, const Node<T>*, bool);

};

template <class T>
void Tree<T>::find(string key){
   if (root == nullptr)
      cout << "EMPTY TREE";
   else
      if (findRecursive(root, key))
         cout << "FOUND";
      else
         cout << "NOT FOUND";
}

template <class T>
bool Tree<T>::findRecursive(Node<T>* curr, string key){
   static bool found = false;

   if (curr == nullptr)
      return false;
   else if (curr->value == key)
      found = true;
   else {
      findRecursive(curr->left, key);
      findRecursive(curr->right, key);
   }
   return found;
}

template <class T>
void Tree<T>::printTree(){
   cout << endl;
   printTree("", root, false);
   cout << endl;
}

template <class T>
void Tree<T>::printTree(const string& prefix, const Node<T>* curr, bool isLeft){
   if (curr != nullptr){
      cout << prefix << (isLeft ? "|--" : "|__") << curr->value << endl;
      printTree(prefix + (isLeft ? "|   " : "    "), curr->right, true);
      printTree(prefix + (isLeft ? "|   " : "    "), curr->left, false);
   }
}

template <class T>
bool Tree<T>::isTerminal(Node<T>* lhs, Node<T>* rhs){
   if (lhs == nullptr && rhs == nullptr)
      return false;
   else if (!(lhs == nullptr)){
      if (lhs->value == "TY" || lhs->value == "ID" || lhs->value == "(" || lhs->value == ")" || lhs->value == "+" || lhs->value == "-" || lhs->value == "*" || lhs->value == "/" || lhs->value == "=" || lhs->value == "e" || lhs->value == "NUM")
         return true;
   }
   else if (!(rhs == nullptr)){
      if (rhs->value == "TY" || rhs->value == "ID" || rhs->value == "(" || rhs->value == ")" || rhs->value == "+" || rhs->value == "-" || rhs->value == "*" || rhs->value == "/" || rhs->value == "=" || rhs->value == "e" || rhs->value == "NUM")
          return true;
   }
   else
      return false;
}

template <class T>
void Tree<T>::insert(T n){
   Node<T>* curr = new Node<T>(n);
   bool yes = true;

   if (root == nullptr)
      root = curr;
   else
      insertRecursive(root, curr, yes);
}

template <class T>
void Tree<T>::insertRecursive(Node<T>* curr, Node<T>* key, bool& yes){
   if (curr == nullptr || isTerminal(curr->left, curr->right))
      return;

   else if (curr->left == nullptr && yes){
      curr->left = key;
      yes = false;
   }
   
   else if (curr->right == nullptr && yes){
      curr->right = key;
      yes = false;
   }

   insertRecursive(curr->left, key, yes);
   insertRecursive(curr->right, key, yes);
}

// **************************************************************************************************
// NOTE BINARY TREE STRUCTURE IMPLEMENTATION FROM CPSC 131 REFERENCE TO HOMEWORK 8 EXCEPT printTree()
// NOTE FUNCTION: printTree()
//    AUTHOR: Adrian Schneider
//    LINK: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
// **************************************************************************************************
