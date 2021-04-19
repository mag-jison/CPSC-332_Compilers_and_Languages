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
   Node<T>* parent;
   vector<Node<T>*> child;

   Node (T v) {
      value = v;
      parent = nullptr;
      child.push_back(nullptr);
   }

   Node(T v, vector<T> rhs) {
       value = v;
       parent = nullptr;
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
    Tree(){
        this->root = nullptr;
    }

    void print() const;
    void printRecursive(Node<T>*) const;
    void insert(T, vector<T>);
    void insertRecursive(Node<T>*, Node<T>*, Node<T>*);
};

template <class T>
void Tree<T>::print() const {
    cout << "\nParse Tree (Pre-Order Traversal)\n-----------------------\n";
    if (root == nullptr)
        cout << "EMPTY TREE!" << endl;
    else
        printRecursive(root);
}

template <class T>
void Tree<T>::printRecursive (Node<T>* curr) const {
    cout << "[ " << curr->value << " ] -> "; 
    for (int i = 0; i < curr->child.size(); i++) {
        if (curr->child[i] != nullptr)
            printRecursive(curr->child[i]);
        else
            cout << "nullptr" << endl;
    }
}

template <class T>
void Tree<T>::insert(T n, vector<T> rhs){
    Node<T>* curr = new Node<T>(n, rhs);

    if (root == nullptr)
        root = curr;
    else
        insertRecursive(root, root, curr);
}

template <class T>
void Tree<T>::insertRecursive(Node<T>* curr, Node<T>* prev, Node<T>* key){
    for (int i = 0; i < curr->child.size(); i++) {
        curr->parent = prev;
        if (curr->child[i] == nullptr)
            return;
        else if (curr->child[i]->value == key->value)
            curr->child[i] = key;
        else
            insertRecursive(curr->child[i], curr, key);
    }
}
