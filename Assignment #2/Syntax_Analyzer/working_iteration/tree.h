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
      root = nullptr;
   }

   void replace(T);
   void replaceNode(Node<T>*, T);
   
   void find(string);
   bool findRecursive(Node<T>*, string);

   void insert(T);
   void insertRecursive(Node<T>*, Node<T>*);

   void preOrder();
   void preOrderTraversal(Node<T>*);

   void printTree();
   void printTree(const string&, const Node<T>*, bool);

};

template <class T>
void Tree<T>::replace(T k){
   replaceNode(root, k);
}

template <class T>
void Tree<T>::replaceNode(Node<T>* curr, T key){
   if (curr == nullptr)
      cout << "";
   else if (curr->value.first == key.first)
      curr->value.second = key.second;
   else {
      replaceNode(curr->left, key);
      replaceNode(curr->right, key);
   }
}

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
   else if (curr->value.first == key)
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
   string line;

   if (curr != nullptr){
      line.assign(25-curr->value.first.length()-prefix.length(), '.');
      cout << prefix;
      cout << (isLeft ? "|--" : "|__");
      cout << curr->value.first.substr(1) << line << "> " << curr->value.second << endl;
      printTree(prefix + (isLeft ? "|   " : "    "), curr->right, true);
      printTree(prefix + (isLeft ? "|   " : "    "), curr->left, false);
   }
}

template <class T>
void Tree<T>::insert(T k){
    Node<T>* curr = new Node<T>(k);

    if (root == nullptr)
        root = curr;
    else
        insertRecursive(root, curr);

}

template <class T>
void Tree<T>::insertRecursive(Node<T>* curr, Node<T>* node){
   // pre-order traversal until reached first non-terminal without a terminal
   if (node->value < curr->value){
       if (curr->left == nullptr)
           curr->left = node;
       else
           insertRecursive(curr->left, node);
   }

   else {
       if (curr->right == nullptr)
           curr->right = node;
       else
           insertRecursive(curr->right, node);
   }      
}

template <class T>
void Tree<T>::preOrder(){
   cout << endl;
   preOrderTraversal(root);
}

template <class T>
void Tree<T>::preOrderTraversal(Node<T>* curr){
   if (curr == nullptr)
      return;

   if (!(curr->left == nullptr) && curr->right == nullptr)
      cout << curr->value.first.substr(1) << setw(8-curr->value.first.substr(1).length()) << right << " -> " << curr->left->value.first.substr(1) << " " << curr->value.second << endl;

   else if (!(curr->right == nullptr) && curr->left == nullptr)
      cout << curr->value.first.substr(1) << setw(8-curr->value.first.substr(1).length()) << right << " -> " << curr->right->value.first.substr(1) << " " << curr->value.second << endl;

   else if (!(curr->left == nullptr && curr->right == nullptr))
      cout << curr->value.first.substr(1) << setw(8-curr->value.first.substr(1).length()) << right << " -> " << curr->left->value.first.substr(1) << " " << curr->value.second << " " << curr->right->value.first.substr(1) << endl;

   else
      cout << curr->value.first.substr(1) << setw(8-curr->value.first.substr(1).length()) << right << " -> " <<  curr->value.second << endl;

   preOrderTraversal(curr->left);
   preOrderTraversal(curr->right);
}

// **************************************************************************************************
// NOTE BINARY TREE STRUCTURE IMPLEMENTATION FROM CPSC 131 REFERENCE TO HOMEWORK 8 EXCEPT printTree()
// NOTE FUNCTION: printTree()
//    AUTHOR: Adrian Schneider
//    LINK: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
// **************************************************************************************************
