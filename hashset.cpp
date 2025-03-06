#include <string>
#include "hashset.h"
#include <iostream>
#include "murmurhash/MurmurHash3.h"

uint64_t get_murmur_hashcode(void *ptr)
{
    uint64_t hash_output;
    uint32_t seed = 42;
    MurmurHash3_x86_32(ptr, strlen(static_cast<char *>(ptr)), seed, &hash_output);
    return hash_output;
}

size_t modular_hash(void *ptr)
{
    return std::hash<void *>{}(ptr);
}

HashSet::HashSet()
{
    size = 4093;
    occupiedSlots = 0;
    items = new Node *[size]();
}

HashSet::~HashSet() {
    for (size_t i = 0; i < size; i++) {
        Node* head = items[i];
        while (head) {
            Node* next = head->next;
            delete head;  // Free memory
            head = next;
        }
    }
}

bool HashSet::insert(void *ptr)
{
    size_t hashValue = modular_hash(ptr);
    size_t index = hashValue % size;

    // Check if already present
    Node *head = items[index];
    while (head != nullptr)
    {
        if (head->getKey() == ptr)
        {
            return false;
        }
        head = head->next;
    }
    // Add to head
    Node* newKey = new Node(ptr, hashValue);
    newKey->next = items[index];
    items[index] = newKey;

    occupiedSlots++;
    if (isEligibleForRehashForInsert())
        rehash(2 * size);
    return true;
}

bool HashSet::lookup(void *ptr)
{
    size_t hashValue = modular_hash(ptr);
    size_t index = hashValue % size;
    Node *head = items[index];
    while (head != nullptr)
    {
        if (head->getKey() == ptr)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

bool HashSet::remove(void *ptr)
{
    if (isEligibleForRehashForRemove())
        rehash(size / 2);
    size_t hashValue = modular_hash(ptr);
    // size_t hashValue = modular_hash(ptr,size);
    size_t index = hashValue % size;
    Node *head = items[index];
    Node *prev = nullptr;
    while (head != nullptr)
    {
        if (head->getKey() == ptr)
        {
            if (prev != nullptr)
            {
                prev->next = head->next;
            }
            else
            {
                items[index] = head->next;
            }
            delete head;
            head = nullptr;
            occupiedSlots--;
            return true;
        }
        prev = head;
        head = head->next;
    }
    return false;
}

bool HashSet::isEligibleForRehashForInsert()
{
    if ((size / 2) < occupiedSlots)
        return true;
    return false;
}

bool HashSet::isEligibleForRehashForRemove()
{
    if ((size / 8) > occupiedSlots)
        return true;
    return false;
}

void HashSet::rehash(size_t newLen)
{
    Node **newItems = new Node *[newLen]();
    for (int it = 0; it < size; it++)
    {
        Node *head = items[it];
        while (head)
        {
            Node *next = head->next;
            size_t newIndex = head->getHashValue() % size;
            head->next = newItems[newIndex];
            newItems[newIndex] = head;
            head = next;
        }
    }
    delete[] items;
    items = newItems;
    size = newLen;
}
