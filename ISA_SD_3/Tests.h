#pragma once

#include <vector>

using namespace std;

class Test {
public:
    static const int REPETITIONS;  // powtórzeń na rozmiar
    static const int SEED;         // stały seed generatora
    static const int COPIES;       // operacji na jedno powtórzenie
    static const vector<int> SIZES;        // badane rozmiary struktur

    //HashTableChaining
    static void testChainingInsert();
    static void testChainingRemove();

    //HashTableAVL
    static void testAVLInsert();
    static void testAVLRemove();

    //HashTableCuckoo
    static void testOpenInsert();
    static void testOpenRemove();
};