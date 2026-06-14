#include "avlHash.h"

#include <iostream>
#include <iomanip>

using namespace std;


//tablica mieszająca z kubełkami AVL


HashTableAVL::HashTableAVL(size_t cap)
    : buckets(cap), capacity(cap), count(0) {
}

void HashTableAVL::insert(int key, int value) {
    size_t idx = hashFunction(key);
    bool        isNew = buckets[idx].insert(key, value);
    if (isNew) ++count;
}

void HashTableAVL::remove(int key) {
    size_t idx = hashFunction(key);
    bool        removed = buckets[idx].remove(key);
    if (!removed)
        throw out_of_range("Klucz nie istnieje: " + to_string(key));
    --count;
}

int HashTableAVL::find(int key) const {
    size_t idx = hashFunction(key);
    int* val = buckets[idx].find(key);
    if (!val)
        throw out_of_range("Klucz nie istnieje: " + to_string(key));
    return *val;
}

bool HashTableAVL::contains(int key) const {
    return buckets[hashFunction(key)].find(key) != nullptr;
}

size_t HashTableAVL::size()        const { return count; }
size_t HashTableAVL::getCapacity() const { return capacity; }

double HashTableAVL::loadFactor() const {
    return capacity > 0
        ? static_cast<double>(count) / static_cast<double>(capacity)
        : 0.0;
}


//funkcja mieszająca

// "Wartość bezwzględna gwarantuje poprawny indeks dla kluczy ujemnych."
size_t HashTableAVL::hashFunction(int key) const {
    long long k = key;
    if (k < 0) k = -k;
    return static_cast<size_t>(k) % capacity;
}