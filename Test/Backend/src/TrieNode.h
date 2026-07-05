#pragma once

#include <random>
#include <string>
#include <vector>
using namespace std;

template <typename DataObject>
struct TrieNode {
	DataObject* dataPtr;
	char key;
	bool isLeaf;

	vector<TrieNode<DataObject>*> children;

	TrieNode(DataObject* dataPtr, char key, bool isLeaf) : dataPtr(dataPtr), key(key), isLeaf(isLeaf) {}
};

/* Implementation */
