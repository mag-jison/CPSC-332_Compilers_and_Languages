#pragma once

#include <iostream> 
#include <string>
#include <iomanip>
#include <vector>
#include <utility>

using namespace std;

template <class T>
struct Node {
   T value;
   vector<Node*> child;

   Node (T v) {
      value = v;
      child.push_back(nullptr);
   }

   Node(T v, vector<T> rhs) {
       value = v;
       for (T e : rhs) {
           Node<T>* temp = new Node<T>(e);
           child.push_back(temp);
       }
   }
};

template <class T>
class Tree {
private:
   Node<T>* root;

public:
   void insert(T, vector<T>);
   void insertRecursive(Node<T>*, Node<T>*);
   bool IS_TERMINAL(string);
};

template <class T>
bool Tree<T>::IS_TERMINAL(string x) {
    return (x == ";" || x == "," || x == "bool" || x == "int" || x == "float" || x == "=" || x == "+" || x == "-" || x == "*" || x == "/" || x == "(" || x == ")" | x == "a" || x == "b" || x == "c" || x == "1" || x == "2" || x == "3" || x == "5.5" || x == "if" || x == "then" || x == "else" || x == "endif" || x == "while" || x == "do" || x == "whileend" || x == "begin" || x == "end" || x == "<" || x == "<=" || x == "==" || x == "<>" || x == ">=" || x == ">") ? true : false;
}

template <class T>
void Tree<T>::insert(T n, vector<T> rhs){
   Node<T>* curr = new Node<T>(n, rhs);

   if (root == nullptr)
       root = curr;
   else
       insertRecursive(root, curr);

   for (auto x : curr->child)
       cout << x->value;
   cout << endl;
}

template <class T>
void Tree<T>::insertRecursive(Node<T>* curr, Node<T>* key){

    for (int i = 0; i < curr->child.size(); i++) {
        if (curr->child.empty() || IS_TERMINAL(curr->value))
            return;
        else if (curr->value == key->value) {
            curr->child[i] = key;
            key->value = "";
        }
        else
            insertRecursive(curr->child[i], key);
    }
}

