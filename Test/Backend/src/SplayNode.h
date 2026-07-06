#pragma once

template <typename DataObject, typename KeyObject>
struct SplayNode {
	DataObject* dataPtr;
	KeyObject key;
	
	SplayNode<DataObject, KeyObject>* left = nullptr;
	SplayNode<DataObject, KeyObject>* right = nullptr;

	SplayNode(DataObject* dataPtr, KeyObject& key) : dataPtr(dataPtr), key(key) {}

	~SplayNode();
};

template <typename DataObject, typename KeyObject>
SplayNode<DataObject, KeyObject>::~SplayNode() {
	delete dataPtr;
}