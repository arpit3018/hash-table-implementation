#ifndef HASHSET_H
#define HASHSET_H

#include <string>
#include "murmurhash/MurmurHash3.h"

struct Node
{
    void *key;
    size_t hash_value;
    Node *next;

    Node() : key(nullptr), hash_value(0), next(nullptr) {}
    Node(void *ptr, int hv, Node *next) : key(ptr), hash_value(hv), next(next) {}

    void *getKey() { return key; }
    size_t getHashValue() { return hash_value; }
    Node *getNext() { return next; }
};

uint64_t get_murmur_hashcode(void *ptr);

struct HashSet
{
    size_t size;
    size_t occupiedSlots;
    Node **items;

    HashSet();
    // ~HashSet();

    bool insert(void *ptr);
    bool lookup(void *ptr);
    bool remove(void *ptr);

private:
    bool isEligibleForRehashForInsert();
    bool isEligibleForRehashForRemove();
    void rehash(size_t newLen);
};

#endif // HASHSET_H
