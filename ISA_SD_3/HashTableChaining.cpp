#include "HashTableChaining.hpp"

HashTableChaining::HashTableChaining(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

HashTableChaining::~HashTableChaining() {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTableChaining::hashFunction(int key) const {
    int hash = key % capacity;
    return hash < 0 ? hash + capacity : hash;
}

void HashTableChaining::resize() {
    int oldCapacity = capacity;
    Node** oldTable = table;

    capacity *= 2;
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
    size = 0;

    for (int i = 0; i < oldCapacity; ++i) {
        Node* current = oldTable[i];
        while (current != nullptr) {
            insert(current->key, current->value);
            Node* toDelete = current;
            current = current->next;
            delete toDelete; // Czyścimy stare węzły, bo insert tworzy nowe
        }
    }
    delete[] oldTable;
}

bool HashTableChaining::insert(int key, int value) {
    // W metodzie łańcuchowej load factor może być większy, np. > 1.0
    if ((double)size / capacity >= 1.0) {
        resize();
    }

    int index = hashFunction(key);
    Node* current = table[index];

    // Sprawdzenie, czy klucz już istnieje – jeśli tak, aktualizujemy wartość
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return true;
        }
        current = current->next;
    }

    // Dodanie nowego węzła na początek listy (najszybsza operacja)
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    size++;
    return true;
}

bool HashTableChaining::remove(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                // Usuwamy pierwszy element z listy
                table[index] = current->next;
            }
            else {
                // Usuwamy element ze środka lub końca listy
                prev->next = current->next;
            }
            delete current;
            size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false; // Klucz nie został znaleziony
}

bool HashTableChaining::search(int key, int& outValue) const {
    int index = hashFunction(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            outValue = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

int HashTableChaining::getSize() const {
    return size;
}