#pragma once

#include <stdexcept>
#include <vector>
#include <stack>
#include "SplayNode.h"

using namespace std;

template <typename DataObject, typename KeyObject>
class SplayTree {
private:
	SplayNode<DataObject, KeyObject>* head = nullptr;

	SplayNode<DataObject, KeyObject>* rotateLeft(SplayNode<DataObject, KeyObject>* node);
	SplayNode<DataObject, KeyObject>* rotateRight(SplayNode<DataObject, KeyObject>* node);
	SplayNode<DataObject, KeyObject>* removeNode(SplayNode<DataObject, KeyObject>* removalNode);
public:
	SplayTree();
	
	bool insert(DataObject* dataPtr, KeyObject& key);
	bool remove(KeyObject& key);
	DataObject* search(KeyObject& key);
	bool empty() const;
	vector<DataObject*> getPreOrder(SplayNode<DataObject, KeyObject>* head = nullptr);
};

/* Implementation */
template <typename DataObject, typename KeyObject>
SplayTree<DataObject, KeyObject>::SplayTree() {}

template <typename DataObject, typename KeyObject>
bool SplayTree<DataObject, KeyObject>::insert(DataObject* dataPtr, KeyObject& key) {
	// Create the new node and ancestors stack
	auto newNode = new SplayNode<DataObject, KeyObject>(dataPtr, key);
	stack<SplayNode<DataObject, KeyObject>*> ancestors;

	// Handle the case where there is no head
	if (this->head == nullptr) {
		this->head = newNode;
		return true;
	}

	// Insert like a regular BST
	SplayNode<DataObject, KeyObject>* curNode = this->head;
	while (curNode != nullptr) {
		// Add the parent to the stack
		ancestors.push(curNode);

		// Traverse through the tree
		if (key > curNode->key)
			curNode = curNode->right;
		else if (key < curNode->key)
			curNode = curNode->left;
		else {
			delete newNode;
			return false;
		}
	}
	if (key > ancestors.top()->key)
		ancestors.top()->right = newNode;
	else
		ancestors.top()->left = newNode;

	// Traverse backwards through the ancestors stack to push the new node to the head
	SplayNode<DataObject, KeyObject>* parentNode = nullptr;
	while (!ancestors.empty()) {
		// Get the top of the stack and pop
		parentNode = ancestors.top();
		ancestors.pop();

		// Get the correct grandparent's child by reference (== parent) and rotate in the correct direction
		SplayNode<DataObject, KeyObject>*& grandparentsChild = ancestors.empty() ? this->head : (ancestors.top()->right == parentNode ? ancestors.top()->right : ancestors.top()->left);
		grandparentsChild = parentNode->right == newNode ? this->rotateLeft(parentNode) : this->rotateRight(parentNode);
	}
}

template <typename DataObject, typename KeyObject>
SplayNode<DataObject, KeyObject>* SplayTree<DataObject, KeyObject>::rotateLeft(SplayNode<DataObject, KeyObject>* node) {
	// Rotate the node left and return the new central node
	SplayNode<DataObject, KeyObject>* child = node->right;
	node->right = child->left;
	child->left = node;

	return child;
}

template <typename DataObject, typename KeyObject>
SplayNode<DataObject, KeyObject>* SplayTree<DataObject, KeyObject>::rotateRight(SplayNode<DataObject, KeyObject>* node) {
	// Rotate the node right and return the new central node
	SplayNode<DataObject, KeyObject>* child = node->left;
	node->left = child->right;
	child->right = node;

	return child;
}

template <typename DataObject, typename KeyObject>
bool SplayTree<DataObject, KeyObject>::remove(KeyObject& key) {
	// Return false if tree is empty
	if (this->empty())
		return false;

	// Traverse through the tree to find the removal node and parent node
	SplayNode<DataObject, KeyObject>* removalNode = this->head;
	SplayNode<DataObject, KeyObject>* parentNode = nullptr;
	while (removalNode != nullptr) {
		// Check removalNode
		if (removalNode->key == key)
			break;

		// Move down the tree
		parentNode = removalNode;
		removalNode = key > removalNode->key ? removalNode->right : removalNode->left;
	}

	// Return false if removal node was not found
	if (removalNode == nullptr)
		return false;

	// Remove the removal node
	if (parentNode == nullptr)
		this->head = this->removeNode(removalNode);
	else if (parentNode->right == removalNode)
		parentNode->right = this->removeNode(removalNode);
	else if (parentNode->left == removalNode)
		parentNode->left = this->removeNode(removalNode);

	return true;
}

template <typename DataObject, typename KeyObject>
SplayNode<DataObject, KeyObject>* SplayTree<DataObject, KeyObject>::removeNode(SplayNode<DataObject, KeyObject>* removalNode) {
	// Remove no children
	if (removalNode->left == nullptr && removalNode->right == nullptr) {
		delete removalNode;
		return nullptr;
	}

	// Remove one child
	if ((removalNode->left == nullptr || removalNode->right == nullptr) && (removalNode->left == nullptr || removalNode->right == nullptr)) {
		SplayNode<DataObject, KeyObject>* returnNode = removalNode->left == nullptr ? removalNode->left : removalNode->right;
		delete removalNode;
		return returnNode;
	}

	// Find the in-order successor
	SplayNode<DataObject, KeyObject>* inOrderSuccessor = removalNode->right;
	SplayNode<DataObject, KeyObject>* successorParent = removalNode;
	while (inOrderSuccessor != nullptr) {
		if (inOrderSuccessor->left == nullptr)
			break;
		successorParent = inOrderSuccessor;
		inOrderSuccessor = inOrderSuccessor->left;
	}

	// Remove the successor from the tree
	SplayNode<DataObject, KeyObject>*& inOrderSuccessorReference = successorParent->right == inOrderSuccessor ? successorParent->right : successorParent->left;
	inOrderSuccessorReference = inOrderSuccessor->right;

	// Put the in-order successor in place of the removal node
	inOrderSuccessor->left = removalNode->left;
	inOrderSuccessor->right = removalNode->right;
	delete removalNode;

	return inOrderSuccessor;
}

template <typename DataObject, typename KeyObject>
DataObject* SplayTree<DataObject, KeyObject>::search(KeyObject& key) {
	// Loop through and find the node
	stack<SplayNode<DataObject, KeyObject>*> ancestors;
	auto curNode = this->head;
	while (curNode != nullptr) {
		ancestors.push(curNode);
		if (key > curNode->key)
			curNode = curNode->right;
		else if (key < curNode->key)
			curNode = curNode->left;
		else
			break;
	}
	if (curNode == nullptr)
		return nullptr;

	// Traverse backwards through the stack to push the new node to the head
	SplayNode<DataObject, KeyObject>* parentNode = nullptr;
	ancestors.pop();
	while (!ancestors.empty()) {
		// Get the top of the stack and pop
		parentNode = ancestors.top();
		ancestors.pop();

		// Get the correct grandparent's child by reference (== parent) and rotate in the correct direction
		SplayNode<DataObject, KeyObject>*& grandparentsChild = ancestors.empty() ? this->head : (ancestors.top()->right == parentNode ? ancestors.top()->right : ancestors.top()->left);
		grandparentsChild = parentNode->right == curNode ? this->rotateLeft(parentNode) : this->rotateRight(parentNode);
	}

	return curNode->dataPtr;
}

template <typename DataObject, typename KeyObject>
bool SplayTree<DataObject, KeyObject>::empty() const {
	return this->head == nullptr;
}

template <typename DataObject, typename KeyObject>
vector<DataObject*> SplayTree<DataObject, KeyObject>::getPreOrder(SplayNode<DataObject, KeyObject>* head) {
	// Set the head to this->head if not provided
	if (head == nullptr)
		head = this->head;

	// Traverse through the tree and get the data pointers
	vector<DataObject*> dataPtrs;
	vector<DataObject*> leftTree;
	vector<DataObject*> rightTree;
	leftTree = head->left == nullptr ? leftTree : this->getPreOrder(head->left);
	rightTree = head->right == nullptr ? rightTree : this->getPreOrder(head->right);

	dataPtrs.push_back(head->dataPtr);
	dataPtrs.insert(dataPtrs.end(), leftTree.begin(), leftTree.end());
	dataPtrs.insert(dataPtrs.end(), rightTree.begin(), rightTree.end());

	return dataPtrs;
}