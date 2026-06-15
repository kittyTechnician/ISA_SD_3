#include "HashTableOpen.hpp"
#include <cmath>

HashTableOpen::HashTableOpen(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    table = new Bucket[capacity];
}

HashTableOpen::~HashTableOpen() {
    delete[] table;
}

int HashTableOpen::hashFunction(int key) const {
    // Prosta funkcja mieszająca modulo, radzi sobie również z liczbami ujemnymi
    int hash = key % capacity;
    return hash < 0 ? hash + capacity : hash;
}

void HashTableOpen::resize() {
    int oldCapacity = capacity;
    Bucket* oldTable = table;

    capacity *= 2;
    table = new Bucket[capacity];
    size = 0;

    for (int i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == OCCUPIED) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }

    delete[] oldTable;
}

bool HashTableOpen::insert(int key, int value) {
    // Współczynnik załadowania (Load Factor) > 0.7 powoduje powiększenie tablicy
    if ((double)size / capacity >= 0.7) {
        resize();
    }

    int index = hashFunction(key);
    int startIndex = index;
    int firstDeletedIndex = -1;

    do {
        if (table[index].state == EMPTY) {
            // Jeśli znaleźliśmy usunięte miejsce wcześniej, używamy go (optymalizacja)
            int targetIndex = (firstDeletedIndex != -1) ? firstDeletedIndex : index;
            table[targetIndex].key = key;
            table[targetIndex].value = value;
            table[targetIndex].state = OCCUPIED;
            size++;
            return true;
        }
        else if (table[index].state == OCCUPIED) {
            if (table[index].key == key) {
                table[index].value = value; // Aktualizacja wartości dla istniejącego klucza
                return true;
            }
        }
        else if (table[index].state == DELETED) {
            if (firstDeletedIndex == -1) {
                firstDeletedIndex = index;
            }
        }

        index = (index + 1) % capacity; // Próbkowanie liniowe
    } while (index != startIndex);

    // Jeśli tablica jest pełna i nie było miejsca (teoretycznie niemożliwe przy resize)
    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex].key = key;
        table[firstDeletedIndex].value = value;
        table[firstDeletedIndex].state = OCCUPIED;
        size++;
        return true;
    }

    return false;
}

bool HashTableOpen::remove(int key) {
    int index = hashFunction(key);
    int startIndex = index;

    do {
        if (table[index].state == EMPTY) {
            return false; // Klucza nie ma w tablicy
        }
        if (table[index].state == OCCUPIED && table[index].key == key) {
            table[index].state = DELETED; // Oznaczenie jako usunięty
            size--;
            return true;
        }
        index = (index + 1) % capacity;
    } while (index != startIndex);

    return false;
}

bool HashTableOpen::search(int key, int& outValue) const {
    int index = hashFunction(key);
    int startIndex = index;

    do {
        if (table[index].state == EMPTY) {
            return false;
        }
        if (table[index].state == OCCUPIED && table[index].key == key) {
            outValue = table[index].value;
            return true;
        }
        index = (index + 1) % capacity;
    } while (index != startIndex);

    return false;
}

int HashTableOpen::getSize() const {
    return size;
}