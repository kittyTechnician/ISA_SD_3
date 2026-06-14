#include "Menu.h"
#include "Tests.h"
#include <iostream>

using namespace std;

//funkcja do czyszczenia bledow wejscia 
static void clearInput() {
	cin.clear();
	cin.ignore(100, '\n');
}

//funkcja zbierajaca input od uzytkownika
static int menuChoice() {
	int choice;
	while (!(cin >> choice)) {
		clearInput();
		cout << "Niepoprawny wybor, sprobuj ponownie. \n";
	}
	return choice;
}

//funkcja wyswietlajaca menu testow dla wybranej struktury danych
static void testMenu(int method) {
	while (true) {
		cout << "\n--- Wybierz operacje do przetestowania ---\n";
		cout << "1. insert\n";
		cout << "2. remove\n";
		cout << "3. wszystkie\n";
		cout << "0. Powrot\n";

		int testChoice = menuChoice();
		if (testChoice == 0) break;

		switch (method) {
		case 1: // Testy dla AVL
			switch (testChoice) {
			case 1:
				Test::testAVLInsert();
				break;
			case 2:
				Test::testAVLRemove();
				break;
			case 3:
				Test::testAVLInsert();
				Test::testAVLRemove();
				break;
			default:
				cout << "Niepoprawny test\n";
				break;
			}
			break;
		}
	}
}

//funkcja wyswietlajaca glowne menu, pozwalajace na wybor struktury danych do testowania
void menu() {
	while (true) {
		cout << "\n--- Wybierz strukture danych do badania ---\n";
		cout << "1. AVL";
		cout << "2. Kopiec\n";
		cout << "0. Wyjscie\n";

		int methodChoice = menuChoice();
		if (methodChoice == 0) break;
		if (methodChoice >= 1 && methodChoice <= 2) {
			testMenu(methodChoice);
		}
		else {
			cout << "Niepoprawna struktura\n";
		}
	}
}