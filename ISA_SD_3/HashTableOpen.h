#pragma once
#ifndef HASHTABLEOPEN_HPP
#define HASHTABLEOPEN_HPP

class HashTableOpen {
private:
    enum BucketState { EMPTY, OCCUPIED, DELETED };

    struct Bucket {
        int key;
        int value;
        BucketState state = EMPTY;
    };

    Bucket* table;
    int capacity;
    int size;

    int hashFunction(int key) const;
    void resize();

public:
    HashTableOpen(int initialCapacity = 16);
    ~HashTableOpen();

    bool insert(int key, int value);
    bool remove(int key);
    // Opcjonalna metoda do testów w menu/benchmarku
    bool search(int key, int& outValue) const;
    int getSize() const;
};

#endif // HASHTABLEOPEN_HPP