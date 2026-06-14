#pragma once

using namespace std;

//węzeł drzewa AVL
struct AVLNode {
    int      key;
    int      value;
    int      height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k, int v);
};



//zbalansowane drzewo BST (jeden "kubełek" tablicy)
class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    //insert pary (key, value)
    //jeśli klucz istnieje – aktualizuje wartość.
    //true gdy klucz był nowy.
    bool insert(int key, int value);

    //usuwa węzeł o podanym kluczu.
    //true gdy klucz istniał.
    bool remove(int key);

    //fukcja zwracająca wskaźnik na wartość lub nullptr gdy klucza nie ma.
    int* find(int key) const;

    bool empty()      const;
    int  treeHeight() const;

private:
    AVLNode* root;

    //narzędzia balansowania
    int      height(AVLNode* n) const;
    int      balanceFactor(AVLNode* n) const;
    void     updateHeight(AVLNode* n);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* balance(AVLNode* n);

    //rekurencyjne operacje na węzłach
    AVLNode* insertNode(AVLNode* node, int key, int value, bool& inserted);
    AVLNode* removeNode(AVLNode* node, int key, bool& removed);
    AVLNode* findNode(AVLNode* node, int key) const;
    AVLNode* minNode(AVLNode* n)             const;

    //zarządzanie pamięcią
    void freeTree(AVLNode* node);
};