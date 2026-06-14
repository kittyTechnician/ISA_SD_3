#pragma once

#include "AVL.h"

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <string>

using namespace std;


//tablica mieszająca z kubełkami AVL
class HashTableAVL {
public:
    //cap – liczba kubełków (domyślnie 16)
    explicit HashTableAVL(size_t cap = 16);

    //wstawia lub aktualizuje parę (key, value).
    void insert(int key, int value);

    //usuwa parę o podanym kluczu.
    //rzuca out_of_range gdy klucz nie istnieje.
    void remove(int key);

    //zwraca wartość powiązaną z kluczem.
    // rzuca out_of_range gdy klucz nie istnieje.
    int  find(int key) const;

    // sprawdza obecność klucza bez rzucania wyjątku.
    bool contains(int key) const;

    size_t size()        const;
    size_t getCapacity() const;
    double      loadFactor()  const;

private:
    vector<AVLTree> buckets;
    size_t          capacity;
    size_t          count;      //liczba unikalnych kluczy

    // metoda dzielenia: h(k) = |k| mod cap
    size_t hashFunction(int key) const;
};