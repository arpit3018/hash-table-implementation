#ifndef HASHSET_H
#define HASHSET_H

#include <string>
#include "murmurhash/MurmurHash3.h"

struct Node {
    void* key;
    int hash_value;
    Node* next;

    Node() : key(nullptr), hash_value(0), next(nullptr) {}
    Node(void* ptr, int hv, Node* next) : key(ptr), hash_value(hv), next(next) {}

    void* getKey() { return key; }
    int getHashValue() { return hash_value; }
    Node* getNext() { return next; }
};

uint64_t get_murmur_hashcode(void* ptr);

struct HashSet {
    int size;
    int occupiedSlots;
    Node** items;

    HashSet();
    // ~HashSet();
    
    bool insert(void* ptr);
    bool lookup(void* ptr);
    bool remove(void* ptr);

private:
    bool isEligibleForRehashForInsert();
    bool isEligibleForRehashForRemove();
    void rehash(int newLen);
};

#endif // HASHSET_H
