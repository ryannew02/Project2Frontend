#pragma once

#include "TrieNode.h"
#include <stack>

using namespace std;

template <typename DataObject>
class Trie {
private:
	TrieNode<DataObject>* root = nullptr;

	void deleteNode(TrieNode<DataObject>* parentNode, TrieNode<DataObject>* removalNode);
	vector<DataObject*> getLeafNodes(TrieNode<DataObject>* parentNode);
public:
	Trie();

	bool insert(DataObject* dataPtr, string& key);
	bool remove(string& key);
	DataObject* search(string& key);
	vector<DataObject*> searchPrefix(string& prefix);
};

/* Implementation */
template <typename DataObject>
Trie<DataObject>::Trie() {};

template <typename DataObject>
bool Trie<DataObject>::insert(DataObject* dataPtr, string& key) {
	// Traverse through the trie inserting a new node where necessary
	if (root == nullptr)
		this->root = new TrieNode<DataObject>(nullptr, '\0', false);

	auto parent = this->root;
	for (unsigned int i = 0; i < key.length(); i++) {
		// Find the matching child node
		TrieNode<DataObject>* childNode = nullptr;
		for (TrieNode<DataObject>*& tempChildNode : parent->children) {
			if (tempChildNode->key == key[i]) {
				childNode = tempChildNode;
				break;
			}
		}

		// Continue traversal or create new node based on the child node's existence
		if (childNode != nullptr) {
			parent = childNode;
		}
		else {
			childNode = new TrieNode<DataObject>(nullptr, key[i], false);
			parent->children.push_back(childNode);
			parent = childNode;
		}

		// Set the leaf node parameter if it is the end of string
		if (i == key.length() - 1) {
			parent->isLeaf = true;
			parent->dataPtr = dataPtr;
		}
	}
	return true;
}

template <typename DataObject>
bool Trie<DataObject>::remove(string& key) {
	// Loop through to find the node matching the end of this key
	if (root == nullptr)
		return false;

	auto parent = this->root;
	stack<TrieNode<DataObject>*> stk;
	for (unsigned int i = 0; i < key.length(); i++) {
		// Find the matching child node
		TrieNode<DataObject>* childNode = nullptr;
		for (TrieNode<DataObject>*& tempChildNode : parent->children) {
			if (tempChildNode->key == key[i]) {
				childNode = tempChildNode;
				break;
			}
		}

		// Continue traversal or create new node based on the child node's existence
		if (childNode != nullptr) {
			if (parent != this->root)
				stk.push(parent);
			parent = childNode;
		}
		else {
			return false;
		}
	}

	// Check if the value is a leaf
	if (!parent->isLeaf)
		return false;

	// Basic delete if the node has children
	if (parent->children.size() > 0) {
		parent->isLeaf = false;
		return true;
	}

	// Loop back through the stack and delete the highest node that has only one child and is non-leaf
	TrieNode<DataObject>* node = nullptr;
	TrieNode<DataObject>* deletionNode = nullptr;
	while (!stk.empty()) {
		// Pop the stack
		node = stk.top();
		stk.pop();

		// Check the children
		if (node->children.size() == 1 && !node->isLeaf)
			deletionNode = node;
		else
			break;
	}

	// Delete the deletion node
	this->deleteNode(stk.empty() ? this->root : stk.top(), deletionNode);
}

template <typename DataObject>
void Trie<DataObject>::deleteNode(TrieNode<DataObject>* parentNode, TrieNode<DataObject>* removalNode) {
	// Delete all children of the removalNode
	for (unsigned int i = 0; i < removalNode->children.size(); i++) {
		this->deleteNode(removalNode, removalNode->children[i]);
		delete removalNode->children[i];
	}

	// Loop through parentNode children and delete the removal node
	for (unsigned int i = 0; i < parentNode->children.size(); i++) {
		if (parentNode->children[i] == removalNode) {

			delete parentNode->children[i];
			parentNode->children.erase(parentNode->children.begin() + i);
			break;
		}
	}
}

template <typename DataObject>
DataObject* Trie<DataObject>::search(string& key) {
	// Traverse through the trie inserting a new node where necessary
	if (root == nullptr)
		return nullptr;

	auto parent = this->root;
	for (unsigned int i = 0; i < key.length(); i++) {
		// Find the matching child node
		TrieNode<DataObject>* childNode = nullptr;
		for (TrieNode<DataObject>*& tempChildNode : parent->children) {
			if (tempChildNode->key == key[i]) {
				childNode = tempChildNode;
				break;
			}
		}

		// Continue traversal or create new node based on the child node's existence
		if (childNode != nullptr) {
			parent = childNode;
		}
		else {
			return nullptr;
		}
	}

	return parent->isLeaf ? parent->dataPtr : nullptr;
}

template <typename DataObject>
vector<DataObject*> Trie<DataObject>::searchPrefix(string& prefix) {
	// Traverse through the trie
	if (root == nullptr)
		return {};

	auto parent = this->root;
	vector<TrieNode<DataObject>*> suffixes;
	for (unsigned int i = 0; i < prefix.length(); i++) {
		// Find the matching child node
		TrieNode<DataObject>* childNode = nullptr;
		for (TrieNode<DataObject>*& tempChildNode : parent->children) {
			if (tempChildNode->key == prefix[i]) {
				childNode = tempChildNode;
				break;
			}
		}

		// Continue traversal or create new node based on the child node's existence
		if (childNode != nullptr) {
			parent = childNode;
		}
		else {
			return suffixes;
		}
	}

	// Loop through children and find all leaf nodes

}

template <typename DataObject>
vector<DataObject*> Trie<DataObject>::getLeafNodes(TrieNode<DataObject>* parentNode) {

}