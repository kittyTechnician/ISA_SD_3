#include "Tests.h"
//#include "hash_table_chaining.h"
#include "avlHash.h"
//#include "hash_table_cuckoo.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <functional>
#include <vector>

using namespace std;

const int              Test::REPETITIONS = 10;
const int              Test::SEED = 6767;
const int              Test::COPIES = 10;
const vector<int>      Test::SIZES = { 5000, 8000, 10000, 16000, 20000, 40000, 80000, 100000 };


class RandomGenerator {
private:
    mt19937                        generator;
    uniform_int_distribution<int>  distribution;
public:
    RandomGenerator(int seed, int min, int max)
        : generator(seed), distribution(min, max) {
    }
    int getNext() { return distribution(generator); }
};


template <typename MapType>
static void runTest(
    const string& testName,
    const string& fileName,
    function<void(MapType&, int, int)> testedFunction,
    bool isRemove = false)
{
    cout << "\nTest: " << testName << "\n";

    ofstream file(fileName);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku " << fileName << "\n";
        return;
    }

    file << "Rozmiar Testu;Sredni Czas [ns]\n";

    for (int size : Test::SIZES) {
        double totalTime = 0.0;

        for (int reps = 0; reps < Test::REPETITIONS; ++reps) {

            //jedna instancja struktury na całe powtórzenie
            MapType instance;

            vector<int> actionKeys(Test::COPIES);
            vector<int> actionValues(Test::COPIES);

            //zapamiętane klucze potrzebne przy remove (analogia insertedValues)
            vector<int> insertedKeys;
            if (isRemove && size > 0) insertedKeys.reserve(size);

            int currentSeedMod = reps * 100;

            RandomGenerator keyGen(Test::SEED + currentSeedMod, 1, 1000000);
            RandomGenerator valGen(Test::SEED + 1000 + currentSeedMod, 1, size * 5);

            // budowanie struktury przed pomiarem
            for (int j = 0; j < size; ++j) {
                int key = keyGen.getNext();
                int val = valGen.getNext();
                instance.insert(key, val);

                if (isRemove) insertedKeys.push_back(key);
            }

            //przygotowanie COPIES par (key, value) do operacji
            RandomGenerator targetIndexGen(Test::SEED + 2000 + currentSeedMod, 0, size > 0 ? size - 1 : 0);
            RandomGenerator testKeyGen(Test::SEED + 3000 + currentSeedMod, 1, 1000000);
            RandomGenerator testValGen(Test::SEED + 4000 + currentSeedMod, 1, size * 5);

            for (int i = 0; i < Test::COPIES; ++i) {
                if (isRemove && size > 0) {
                    //losujemy istniejący klucz (tak jak isModifyKey losował istniejącą wartość)
                    int randomIndex = targetIndexGen.getNext();
                    actionKeys[i] = insertedKeys[randomIndex];
                }
                else {
                    actionKeys[i] = testKeyGen.getNext();
                }
                actionValues[i] = testValGen.getNext();
            }

            //start pomiaru czasu
            auto start = chrono::high_resolution_clock::now();

            for (int i = 0; i < Test::COPIES; ++i) {
                testedFunction(instance, actionKeys[i], actionValues[i]);
            }

            auto end = chrono::high_resolution_clock::now();
            //koniec pomiaru czasu

            auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            totalTime += static_cast<double>(duration) / Test::COPIES;
        }

        double averageTime = totalTime / Test::REPETITIONS;

        file << size << ";" << static_cast<long long>(averageTime) << "\n";
        cout << "   Rozmiar = " << size
            << " \tzakonczony. Srednia: " << static_cast<long long>(averageTime) << " ns\n";
    }

    file.close();
}

//testy HashTableAVL

void Test::testAVLInsert() {
    runTest<HashTableAVL>(
        "AVL_insert", "AVL_insert.csv",
        [](HashTableAVL& ht, int key, int val) {
            ht.insert(key, val);
        });
}

void Test::testAVLRemove() {
    runTest<HashTableAVL>(
        "AVL_remove", "AVL_remove.csv",
        [](HashTableAVL& ht, int key, int val) {
            if (ht.contains(key)) ht.remove(key);
        }, true);
}
