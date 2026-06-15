#ifndef HASHTABLECHAINING_HPP
#define HASHTABLECHAINING_HPP

class HashTableChaining {
private:
    struct Node {
        int key;
        int value;
        Node* next;
        Node(int k, int v) : key(k), value(v), next(nullptr) {}
    };

    Node** table; // Tablica wskaźników na węzły list
    int capacity;
    int size;

    int hashFunction(int key) const;
    void resize();

public:
    HashTableChaining(int initialCapacity = 16);
    ~HashTableChaining();

    bool insert(int key, int value);
    bool remove(int key);
    // Opcjonalna metoda pomocnicza
    bool search(int key, int& outValue) const;
    int getSize() const;
};

#endif // HASHTABLECHAINING_HPP