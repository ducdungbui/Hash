// HashMap.hpp
//
// ICS 45C Fall 2021
// Project #3: Maps and Legends
//
// This header file contains a declaration for a "well-behaved" HashMap
// class, which is a separately-chained hash table implementation of a
// map/dictionary data structure (i.e., a collection of key/value pairs),
// where the keys and values are strings.
//
// This HashMap consists of a dynamically-allocated array of "buckets,"
// each of which is a pointer to the first node in a linked list of
// key/value pairs that have hashed to that bucket.
//
// You may not make changes to the public members of this class the way
// they're declared now, since we will be running unit tests against
// your class that will expect these declarations not to have changed
// (and that its member functions are implemented as specified in the
// comments below).  However, you are free to add whatever additional
// members you feel are necessary, including new member variables or
// member functions (public or private).

#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <functional>
#include <string>
#include <utility>


class HashMap
{
public:
    // This is called a "type alias," which is a way to define a new
    // type that is a synonym for an existing one.  In this case,
    // we're introducing HashFunction as a new name for a type
    // representing a function that takes a reference to a constant
    // string and returns an unsigned int.  Given that, a variable
    // or parameter of type HashFunction can be assigned such a
    // function.
    //
    // Hash functions must conform to these properties:
    //
    // (1) Given a particular string repeatedly, they must always
    //     return the same hash value.
    // (2) They cannot take the number of buckets into account (as they
    //     do not receive a parameter that tells them how many buckets
    //     there are).  As a result of this, the result can be any
    //     unsigned int value (i.e., the result won't necessarily be a
    //     legitimate index for a bucket.  It will be the job of the
    //     HashMap class to reduce the results to the range of
    //     available bucket indices (e.g., by using the % operator).
    using HashFunction = std::function<unsigned int(const std::string&)>;

    // This constant specifies the number of buckets that a HashMap will
    // have when it is initially constructed.  (A "constexpr" is similar
    // to a variable that's constant, except that it can be defined as
    // any arbitrary expression that can be calculated at compile time.)
    // 
    // We've made it static because it pertains to the entire class
    // rather than each instance of the class separately.  The best way
    // to use this value would be to say "HashMap::INITIAL_BUCKET_COUNT".
    static constexpr unsigned int INITIAL_BUCKET_COUNT = 10;


public:
    // This constructor initializes the HashMap to use whatever default
    // hash function you'd like it to use.  A little research online will
    // yield some good ideas about how to write a good hash function for
    // strings; don't just return zero or, say, the length of the string.
    HashMap();

    // This constructor instead initializes the HashMap to use a particular
    // hash function instead of the default.  (We'll use this in our unit
    // tests to control the scenarios more carefully.)
    HashMap(HashFunction hashFunction);

    // The "Big Three" need to be implemented appropriately, so that HashMaps
    // can be created, destroyed, copied, and assigned without leaking
    // resources, interfering with one another, or causing crashes or
    // undefined behavior.
    HashMap(const HashMap& hm);
    ~HashMap();
    HashMap& operator=(const HashMap& hm);

    // add() takes a key and a value.  If the key is not already stored in
    // this HashMap, the key/value pair is added; if the key is already
    // stored, the function has no effect.
    //
    // If adding the new key/value pair will cause the load factor of this
    // HashMap to exceed 0.8, the following must happen:
    //
    // (1) The number of buckets should be increased by doubling it and
    //     adding 1 (i.e., if there were 10 buckets, increase it to
    //     2 * 10 + 1 = 21).
    // (2) All key/value pairs should be rehashed into their new buckets,
    //     which is important because changing the number of buckets will
    //     likely change the bucket where a particular key is meant to
    //     reside.
    void add(const std::string& key, const std::string& value);

    // remove() takes a key and removes it (and its associated value) from
    // this HashMap if it is already present; if not, nothing is changed.
    // The function returns true if the key was removed, false if nothing
    // was removed.
    bool remove(const std::string& key);

    void clear();

    // contains() returns true if the given key is in this HashMap, false
    // if not.
    bool contains(const std::string& key) const;

    // value() returns the value associated with the given key in this HashMap
    // if the key is stored in this HashMap; if not, the empty string is
    // returned.  (Going forward, we'll discover that throwing an exception
    // is a better way to handle the scenario where the key is not present,
    // but we'll conquer that at a later date.)
    std::string value(const std::string& key) const;

    // size() returns the number of key/value pairs stored in this HashMap.
    unsigned int size() const;

    // bucketCount() returns the number of buckets currently allocated in
    // this HashMap.
    unsigned int bucketCount() const;

    // loadFactor() returns the proportion of the number of key/value pairs
    // to the number of buckets, a measurement of how "full" the HashMap is.
    // For example, if there are 20 key/value pairs and 50 buckets, we would
    // say that the load factor is 20/50 = 0.4.
    double loadFactor() const;

    // maxBucketSize() returns the number of key/value pairs stored in this
    // HashMap's largest bucket.
    unsigned int maxBucketSize() const;


private:
    // This structure describes the nodes that make up the linked lists in
    // each of this HashMap's buckets.
    struct Node
    {
        std::string key;
        std::string value;
        Node* next;
    };

    class LinkedList{

      private:
        Node *_head;
        unsigned int _count=0;

    public:
        LinkedList(){
            _head = NULL;
        }

        Node* get_head(){
            return _head;
        }

        void add_node(std::string key, std::string value)
        {
            Node *tmp = new Node;
            tmp->value = value;
            tmp->key = key;
            tmp->next = NULL;

            if(_head == NULL)
            {
                _head = tmp;
            }
            else
            {
                tmp->next = _head;
                _head = tmp;
            }
            _count++;
        }

        bool delete_node(std::string key)
            {
                Node *current= _head;
                Node *previous= NULL;
        

                   // If head node itself holds
                  // the key to be deleted
                    if (current != NULL && current->key == key)
                    {
                        _head = current->next; // Changed head
                        delete current;            // free old head
                        _count --;
                        return true;
                    } else {

                        while (current != NULL && current->key != key)
                        {
                             previous = current;
                             current = current->next; 
                        }

                        if(current == NULL)
                            return false; //nothing to delete

                        previous->next = current->next;
                        delete current;
                        _count--;
                        return true;

                    }
            }

        std::pair<Node, bool> get_node(std::string key){
            Node *current = _head;
            Node *tmp = new Node;
            
            while(current != NULL && current->key != key){
                current = current->next;
            }


            if(current != NULL)
            {
                return std::make_pair(*current, true);
            }

            return std::make_pair(*tmp, false);
        }

        unsigned int size() const{
            return _count;
        }

    };


    // Store the hash function (either the default hash function or the one
    // passed to the constructor as a parameter) in this member variable.
    // When you want to hash a key, call this member variable (i.e., follow
    // it with parentheses and a parameter) just like you would any other
    // function.
    HashFunction hashFunction;


    // You will no doubt need to add at least a few more private members
    unsigned int _itemsCount = 0;
    unsigned int _bucketCount = INITIAL_BUCKET_COUNT;
    double _loadFactor = 0;
    LinkedList* _buckets = new LinkedList [INITIAL_BUCKET_COUNT];

};



#endif // HASHMAP_HPP
