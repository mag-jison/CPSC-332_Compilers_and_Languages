#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include <iterator>
#include <fstream>

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
            Node<T>* temp = new Node<T>(e); // assigns every child node with its own value
            child.push_back(temp);
        }
    }
};

template <class T>
class Tree {
private:
    Node<T>* root;
    ofstream output;

public:
    Tree(){
        root = nullptr;
    }
    
    Node<T>* getRoot();
    void printTree();
    void BFS(Node<T>*);
    void makeChildren(Node<T>*);
    void preOrderTraversal(Node<T>*);
    void makeTree(T, const vector<T>&);
    void makeTreeRecursive(Node<T>*, Node<T>*, Node<T>*);
};

template <class T>
Node<T>* Tree<T>::getRoot(){
    return root;
}

template <class T>
void Tree<T>::printTree(){
    output.open("files/out.txt");
    output << "+--Parse Tree--+\n\n";

    if (root == nullptr)
        output << "EMPTY TREE!";
    else
        preOrderTraversal(root);

    output << "\n+--------------+\n";
    output.close();
}

template <class T>
void Tree<T>::BFS(Node<T>* curr){
    static int j = -1;
    static int k = -1;

    if (curr == nullptr)
        return;
    else if (curr->value.back() == '~')
        curr->value.pop_back();
    
    makeChildren(curr);

    if (curr->parent->child[++j] != nullptr){
        BFS(curr->parent->child[j]);
    }
    else if (curr->parent->child[++k] != nullptr) {
        BFS(curr->parent->child[k]);
        j = -1;
    }

    return;
}

template <class T>
void Tree<T>::makeChildren(Node<T>* curr){
    output << "[ " << curr->value << " ] -> [ "; 
    for (typename vector<Node<T>*>::iterator it = curr->child.begin(); it != curr->child.end(); ++it) {
        if (*it != nullptr){
            if ((*it)->value.back() == '~')
                (*it)->value.pop_back();
            output << (*it)->value << " ";
        }
        else
            output << "nullptr ";
    }
    output << "]\n";
}

template <class T>
void Tree<T>::preOrderTraversal(Node<T>* curr){
    if (curr->value.back() == '~')
        curr->value.pop_back();
    
    if (curr->parent != nullptr)
        output << curr->parent->value << "::"; 

    output << "[ " << curr->value << " ] -> ";
    for (size_t i = 0; i < curr->child.size(); ++i) {
        if (curr->child[i] != nullptr)
            preOrderTraversal(curr->child[i]);
        else
            output << "nullptr\n";
    }
    return;
}

template <class T>
void Tree<T>::makeTree(T n, const vector<T>& rhs){
    Node<T>* curr = new Node<T>(n, rhs);

    if (root == nullptr)
        root = curr;
    else
        makeTreeRecursive(root, root, curr);
}

template <class T>
void Tree<T>::makeTreeRecursive(Node<T>* curr, Node<T>* prev, Node<T>* key){
    for (size_t i = 0; i < curr->child.size(); ++i) {
        curr->parent = prev;
        if (curr->child[i] == nullptr)
            return;
        else if (curr->child[i]->value == key->value && curr->child[i]->value.back() != '~'){
            curr->child[i] = key;
            curr->child[i]->value.push_back('~');
        }
        else
            makeTreeRecursive(curr->child[i], curr, key);
    }
}
