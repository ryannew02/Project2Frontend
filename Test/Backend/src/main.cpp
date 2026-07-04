#include <iostream>
#include <string>
#include "SplayTree.h"
#include "Location.h"

using namespace std;

void print(SplayTree<Location, string> tree);

int main() {
	SplayTree<Location, string> splayTree;

	Location loc1("1313 N Collier Blvd", 5.8f, 5.2f);
	Location loc2("4365 Kauila St", 2.4f, 8.1f);
	Location loc3("2090 Hanalima St", 1.8f, 9.4f);

	splayTree.insert(&loc1, loc1.address);
	print(splayTree);
	splayTree.insert(&loc2, loc2.address);
	print(splayTree);
	splayTree.insert(&loc3, loc3.address);
	print(splayTree);

	return 0;
}

void print(SplayTree<Location, string> tree) {
	auto dataPtrs = tree.getPreOrder();
	for (unsigned int i = 0; i < dataPtrs.size(); i++)
		cout << dataPtrs[i]->address << endl;
	cout << endl << endl;
}