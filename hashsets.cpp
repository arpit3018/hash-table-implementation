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
    static const int SIZE = 1024;
    public:
        Node* items[SIZE];

    bool insert(void* ptr) {
        int hashValue = get_murmur_hashcode(ptr);
        int index = hashValue % SIZE;
    
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
        return true;
    }

    bool lookup(void* ptr) {
        int hashValue = get_murmur_hashcode(ptr);
        int index = hashValue % SIZE;
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
        int index = hashValue % SIZE;
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
                return true;
            }
            prev = head;
            head = head->next;
        }
        return false;
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

