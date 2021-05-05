#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include <iterator>

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
        root = nullptr;
    }
    
    Node<T>* getRoot();
    T printTree();
    T BFS(Node<T>*);
    T preOrderTraversal(Node<T>*);
    void storeTree(T, const vector<T>&);
    void storeTreeRecursive(Node<T>*, Node<T>*, Node<T>*);
};

template <class T>
Node<T>* Tree<T>::getRoot(){
    return root;
}

template <class T>
T Tree<T>::printTree(){
    return (root == nullptr) ? "EMPTY TREE!" : preOrderTraversal(root);
}

template <class T>
T Tree<T>::BFS(Node<T>* curr){
    static int j = 0;

    cout << "[ " << curr->value << " ] -> [ "; 
    for (typename vector<Node<T>*>::iterator it = curr->child.begin(); it != curr->child.end(); ++it) {
        if (*it != nullptr){
            if ((*it)->value.back() == '~')
                (*it)->value.pop_back();
            cout << (*it)->value << " ";
        }
        else
            cout << "nullptr ";
    }
    cout << "]\n";

    if (!root->child[j++]->child.empty())
        BFS(root->child[j]);

    return "";
}

template <class T>
T Tree<T>::preOrderTraversal(Node<T>* curr){

    if (curr->value.back() == '~')
        curr->value.pop_back();
    
    if (curr->parent != nullptr)
        cout << curr->parent->value << "::"; 

    cout << "[ " << curr->value << " ] -> ";
    for (size_t i = 0; i < curr->child.size(); ++i) {
        if (curr->child[i] != nullptr)
            preOrderTraversal(curr->child[i]);
        else
            cout << "nullptr\n";
    }
    return "";
}

template <class T>
void Tree<T>::storeTree(T n, const vector<T>& rhs){
    Node<T>* curr = new Node<T>(n, rhs);

    if (root == nullptr)
        root = curr;
    else
        storeTreeRecursive(root, root, curr);
}

template <class T>
void Tree<T>::storeTreeRecursive(Node<T>* curr, Node<T>* prev, Node<T>* key){
    for (size_t i = 0; i < curr->child.size(); ++i) {
        curr->parent = prev;
        if (curr->child[i] == nullptr)
            return;
        else if (curr->child[i]->value == key->value && curr->child[i]->value.back() != '~'){
            curr->child[i] = key;
            curr->child[i]->value.push_back('~');
        }
        else
            storeTreeRecursive(curr->child[i], curr, key);
    }
}
