#include "AVL.h"

#include <iostream>
#include <algorithm>

using namespace std;

AVLNode::AVLNode(int k, int v)
    : key(k), value(v), height(1), left(nullptr), right(nullptr) {
}



//drzewo AVL

AVLTree::AVLTree() : root(nullptr) {}
AVLTree::~AVLTree() { freeTree(root); }

bool AVLTree::insert(int key, int value) {
    bool inserted = false;
    root = insertNode(root, key, value, inserted);
    return inserted;
}

bool AVLTree::remove(int key) {
    bool removed = false;
    root = removeNode(root, key, removed);
    return removed;
}

int* AVLTree::find(int key) const {
    AVLNode* n = findNode(root, key);
    return n ? &n->value : nullptr;
}

bool AVLTree::empty()      const { return root == nullptr; }
int  AVLTree::treeHeight() const { return height(root); }



//narzędzia balansowania

int AVLTree::height(AVLNode* n) const {
    return n ? n->height : 0;
}

int AVLTree::balanceFactor(AVLNode* n) const {
    return n ? height(n->left) - height(n->right) : 0;
}

void AVLTree::updateHeight(AVLNode* n) {
    if (n)
        n->height = 1 + max(height(n->left), height(n->right));
}

//rotacja prawa (przypadek LL)
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);
    return x;
}

//rotacja lewa (przypadek RR)
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);
    return y;
}

//przywrócenie własności AVL po modyfikacji poddrzewa
//wszystkie cztery przypadki: LL, LR, RR, RL.
AVLNode* AVLTree::balance(AVLNode* n) {
    updateHeight(n);
    int bf = balanceFactor(n);

    //LL
    if (bf > 1 && balanceFactor(n->left) >= 0)
        return rotateRight(n);

    //LR
    if (bf > 1 && balanceFactor(n->left) < 0) {
        n->left = rotateLeft(n->left);
        return rotateRight(n);
    }

    //RR
    if (bf < -1 && balanceFactor(n->right) <= 0)
        return rotateLeft(n);

    //RL
    if (bf < -1 && balanceFactor(n->right) > 0) {
        n->right = rotateRight(n->right);
        return rotateLeft(n);
    }

    return n;
}


//rekurencja na węzłach – O(log n)

AVLNode* AVLTree::insertNode(AVLNode* node, int key, int value, bool& inserted) {
    if (!node) {
        inserted = true;
        return new AVLNode(key, value);
    }

    if (key < node->key)
        node->left = insertNode(node->left, key, value, inserted);
    else if (key > node->key)
        node->right = insertNode(node->right, key, value, inserted);
    else {
        //klucz istnieje – aktualizuje wartość węzła
        node->value = value;
        inserted = false;
        return node;
    }

    return balance(node);
}

AVLNode* AVLTree::removeNode(AVLNode* node, int key, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    if (key < node->key)
        node->left = removeNode(node->left, key, removed);
    else if (key > node->key)
        node->right = removeNode(node->right, key, removed);
    else {
        removed = true;

        //zero lub jeden potomek
        if (!node->left || !node->right) {
            AVLNode* child = node->left ? node->left : node->right;
            delete node;
            return child;
        }

        //dwóch potomków – zastępujemy następnikiem in-order
        AVLNode* successor = minNode(node->right);
        node->key = successor->key;
        node->value = successor->value;

        bool dummy = false;
        node->right = removeNode(node->right, successor->key, dummy);
    }

    return balance(node);
}

//wyszukiwanie – O(log n)
AVLNode* AVLTree::findNode(AVLNode* node, int key) const {
    while (node) {
        if (key < node->key) node = node->left;
        else if (key > node->key) node = node->right;
        else                      return node;
    }
    return nullptr;
}

//węzeł skrajnie lewy
AVLNode* AVLTree::minNode(AVLNode* n) const {
    while (n->left) n = n->left;
    return n;
}

//zarządzanie pamięcią

void AVLTree::freeTree(AVLNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

