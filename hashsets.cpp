#include <string>
#include <iostream>
#include "murmurhash/MurmurHash3.h"

struct Node {
    void* key;
    int hash_value;
    struct Node * next;
    public:
        Node(){}
        Node(void* ptr, int hv, Node* next) : key(ptr), hash_value(hv), next(next) {}
        void* getKey() {return key;}
        int getHashValue() {return hash_value;}
        Node* getNext() {return next;}
};


uint64_t get_murmur_hashcode(void* ptr){
    uint64_t hash_output;
    uint32_t seed = 42;
    MurmurHash3_x86_32(ptr, strlen(static_cast<char*>(ptr)), seed, &hash_output);
    return hash_output;
}

struct HashSet {
    int size;
    int occupiedSlots;
    Node** items;

    HashSet() {
        size = 1024;
        occupiedSlots = 0;
        items = new Node*[size];
    }

    bool insert(void* ptr) {
        int hashValue = get_murmur_hashcode(ptr);
        int index = hashValue & (size-1);
    
        if(items[index] == nullptr) {
            Node* newKey = new Node(ptr,hashValue,nullptr);
            items[index] = newKey;
        } else {
            //Check if already present
            Node* head = items[index];
            while(head != nullptr) {
                if(head->getKey() == ptr) {
                    return false;
                }
                head = head->next;
            }
            //Add to head
            Node* newKey = new Node(ptr,hashValue,nullptr);
            newKey->next = items[index];
            items[index] = newKey; 
        }
        occupiedSlots++;
        if(isEligibleForRehashForInsert()) rehash(2*size);
        return true;
    }

    bool lookup(void* ptr) {
        int hashValue = get_murmur_hashcode(ptr);
        int index = hashValue & (size-1);
        Node* head = items[index];
        while(head != nullptr) {
            if(head->getKey() == ptr) {
                return true;
            }
            head = head->next;
        }
        return false;
    }

    bool remove(void* ptr) {
        int hashValue = get_murmur_hashcode(ptr);
        int index = hashValue & (size-1);
        Node* head = items[index];
        Node* prev = nullptr;
        while(head != nullptr) {
            if(head->getKey() == ptr) {
                if(prev != nullptr) {
                    prev->next = head->next;
                } else {
                    items[index] = head->next;
                }                
                delete head;
                head = nullptr;
                occupiedSlots--;
                if(isEligibleForRehashForRemove()) rehash(size/2);
                return true;
            }
            prev = head;
            head = head->next;
        }
        return false;
    }

    bool isEligibleForRehashForInsert() {      
        float loadFactor = (float)occupiedSlots / size;
        if(loadFactor > 0.5) return true;
        return false;        
    }

    bool isEligibleForRehashForRemove() {      
        float loadFactor = (float)occupiedSlots / size;
        if(loadFactor < 0.125) return true;
        return false;        
    }

    void rehash(int newLen) {
        Node** newItems = new Node*[newLen];
        for(int it=0;it<size;it++) {
            if(items[it] == nullptr) continue;
            int hash = items[it]->getHashValue();
            int newIndex = hash & (newLen-1);
            newItems[newIndex] = items[it];
        }           
        delete items;
        items = newItems;
        size = newLen;
    }
    
};

int main() {
    HashSet* hashset = new HashSet();
    std::string foo = "foo";
    std::string missing = "missing";
    std::cout<<"Insert Foo : "<<std::boolalpha<<hashset->insert(&foo)<<std::endl;
    std::cout<<"Lookup Foo : "<<std::boolalpha<<hashset->lookup(&foo)<<std::endl;
    std::cout<<"Lookup Missing : "<<std::boolalpha<<hashset->lookup(&missing)<<std::endl;
    std::cout<<"Remove Missing : "<<std::boolalpha<<hashset->remove(&missing)<<std::endl;
    std::cout<<"Remove Foo : "<<std::boolalpha<<hashset->remove(&foo)<<std::endl;
    std::cout<<"Lookup Foo : "<<std::boolalpha<<hashset->lookup(&foo)<<std::endl;

    delete hashset;
    return 0;

}

