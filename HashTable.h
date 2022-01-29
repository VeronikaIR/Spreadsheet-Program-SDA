#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <set>
#include <functional>
#include <fstream>

using std::string;
using std::list;
using std::vector;
using std::pair;

const size_t MAX_CAPACITY = 4194304; // calculated based on rough estimation of how big a file can be

template <typename T>
size_t stdHashFunctionWrapper(const T& obj) {
	return std::hash<T>{}(obj);
}

template <typename K, typename V>
class HashTable {

private:

	vector<list<pair<K, V>>*> container;

	size_t capacity = MAX_CAPACITY;
	size_t size = 0;

	size_t(*hashFunc)(const K&) = &stdHashFunctionWrapper;

public:
	HashTable() {
		this->container.resize(this->capacity);
	}

	HashTable(size_t(*currHashFunc)(const K&)) {
		this->container.resize(this->capacity);
		this->hashFunc = currHashFunc;
	}

	HashTable(const size_t& currCapacity) {

		if (currCapacity == 0) {
			this->capacity = MAX_CAPACITY;
		}

		if (currCapacity < MAX_CAPACITY) {
			this->capacity = currCapacity;
		}
		this->container.resize(this->capacity);
	}

	HashTable(const size_t& currCapacity, size_t(*currHashFunc)(const K&)) {

		if (currCapacity == 0) {
			this->capacity = MAX_CAPACITY;
		}

		if (currCapacity < MAX_CAPACITY) {
			this->capacity = currCapacity;
		}
		this->container.resize(this->capacity);
		this->hashFunc = currHashFunc;
	}

	void put(const K& key, const V& value){

		size_t indx = this->hashFunc(key) % this->capacity;

		if (container[indx] == nullptr) {
			pair<K, V> currPair(key, value);
			list<pair<K, V>>* currList = new list<pair<K, V>>();
			container[indx] = currList;
			currList->push_front(currPair);
			size++;
		}
		else {
			bool foundKey = false;
			for (auto it = container[indx]->begin(); it != container[indx]->end(); it++) {
				if ((*it).first == key) {
					(*it).second = value;
					foundKey = true;
					break;
				}
			}
			if (foundKey == false) {
				pair<K, V> currPair(key, value);
				container[indx]->push_front(currPair);
				size++;
			}
		}
	}

	bool get(const K& key, V& value)const {

		size_t indx = this->hashFunc(key) % this->capacity;
		if (container[indx] != nullptr) {
			for (auto it = container[indx]->begin(); it != container[indx]->end(); it++) {
				if ((*it).first == key) {
					value = (*it).second;
					return true;
				}
			}
		}
		return false;
	}

	size_t getSize()const {

		return this->size;
	}

	void remove(const K& key) {

		size_t indx = this->hashFunc(key) % this->capacity;

		if (container[indx] == nullptr) {
			return;
		}
		else {
			for (auto it = container[indx]->begin(); it != container[indx]->end(); it++) {
				if ((*it).first == key) {
					if (container[indx]->begin() == it) {
						container[indx]->pop_front();
					}
					else {
						container[indx]->erase(it--);
					}
					this->size--;
					break;
				}
			}
		}
	}

	vector<pair<K, V>> getAllKeyValues()const {
		vector<pair<K, V>> allKeysValues;
		size_t containerSize = container.size();
		for (int i = 0; i < containerSize; i++) {
			if (container[i] != nullptr) {
				for (auto it = container[i]->begin(); it != container[i]->end(); it++) {
					allKeysValues.push_back(*it);
				}
			}
		}
		return allKeysValues;
	}
};

