#include "hashmap.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector; using std::istringstream;
using std::stringstream;

int main(){
    HashMap map;

   bool debug = false;
   std::string input;
   int commandsSize;
   bool commandValid = false;
   while(true)
   {
       getline (cin, input);
       commandValid = false;
        vector<string> commands{};
        std::istringstream iss(input);
        for (std::string s; iss >> s; )
        {
            if(s != "")
                commands.push_back(s);
        }
        commandsSize = commands.size();
        if(commandsSize > 0 && commandsSize <=3) //we could only have up to 3 arguments
        {
            if(commands[0] == "QUIT" && commandsSize == 1)
            {
                cout << "GOODBYE" << endl;
                break;
            }
            else if(commands[0] == "DEBUG")
            {
                if(commandsSize == 2)
                {
                    if(commands[1] == "ON")
                    {
                        commandValid = true;
                        if(debug){
                            cout <<"ON ALREADY" <<endl;
                        } else {
                            debug = true;
                            cout <<"ON NOW" <<endl;
                        }
                    } else if(commands[1] == "OFF"){
                        commandValid = true;
                        if(debug){
                            debug = false;
                            cout <<"OFF NOW" <<endl;
                        } else {
                            
                            cout <<"OFF ALREADY" <<endl;
                        }

                    }

                }

            } 
            else if(commands[0] == "LOGIN" && debug && commandsSize == 2)
            {
                if(commands[1] == "COUNT"){
                    commandValid = true;
                    cout << map.size()<<endl;
                }
            }
            else if(commands[0] == "BUCKET" && debug && commandsSize == 2)
            {
                if(commands[1] == "COUNT"){
                    commandValid = true;
                    cout << map.bucketCount()<<endl;
                }
            }
            else if(commands[0] == "LOAD" && debug && commandsSize == 2)
            {
                if(commands[1] == "FACTOR"){
                    commandValid = true;
                    cout << map.loadFactor()<<endl;
                }
            }
            else if(commands[0] == "MAX" && debug && commandsSize == 3)
            {
                if(commands[1] == "BUCKET" && commands[2] == "SIZE"){
                    commandValid = true;
                    cout << map.maxBucketSize()<<endl;
                }
            }
            else if(commands[0] == "LOGIN" && commandsSize == 3){
                commandValid = true;
                string pass = map.value(commands[1]);
                if(pass == commands[2]){
                    cout << "SUCCEEDED" << endl;
                } else {
                    cout << "FAILED" << endl;
                }
            }
            else if(commands[0] == "REMOVE" && commandsSize == 2){
                commandValid = true;
                if( map.remove(commands[1])){
                    cout <<"REMOVED"<<endl;
                } else {
                    cout <<"NONEXISTENT"<<endl;
                }
            }
            else if(commands[0] == "CREATE" && commandsSize == 3){
                commandValid = true;
                if(map.contains(commands[1])){
                    cout <<"EXISTS"<<endl;
                } else {
                    map.add(commands[1], commands[2]);
                    cout <<"CREATED"<<endl;
                }
            }
            else if(commands[0] == "CLEAR" && commandsSize == 1){
                commandValid = true;
                map.clear();
                cout <<"CLEARED"<<endl;
            }
        } 

        if(!commandValid)
        {
            cout << "INVALID" <<endl;
        }

   }
}

unsigned int hash_function(const std::string &key){
    unsigned int fnvPrime = 1099511628211;
    unsigned int hashVal = 14695981039346656037;
    int len;
    
    for (int i = 0; i < key.length(); ++i)
    {
        hashVal ^= key[i];
        hashVal *= fnvPrime;
    }

    return hashVal;
}

HashMap::HashMap()
{
   hashFunction = hash_function;
}


HashMap::HashMap(HashFunction func){
    hashFunction = func;
}
HashMap:: HashMap(const HashMap& hm){

}
HashMap::~HashMap(){
delete [] _buckets;
}

void HashMap::clear(){
    
    if(_itemsCount == 0 && _bucketCount == INITIAL_BUCKET_COUNT) return; //no need to do anything

    delete [] _buckets; //clear memory
    _buckets = new LinkedList[INITIAL_BUCKET_COUNT];
    _bucketCount = INITIAL_BUCKET_COUNT;
    _itemsCount = 0;
    _loadFactor = 0;
}

void HashMap::add(const std::string& key, const std::string& value)
{
    if(key.empty() || value.empty()) return; // do nothing

    unsigned int bucketIndex = hashFunction(key) % _bucketCount;
     
    std::pair<Node, bool> result = _buckets[bucketIndex].get_node(key);

    if(!result.second){

     //add
     double loadFactor = (double)(_itemsCount + 1) /(double) _bucketCount;
     if(loadFactor > 0.8){
        //need to rebalance the buckets
        unsigned int newBucketCount = _bucketCount * 2 + 1;
        unsigned int newBucketIndex;
        LinkedList *newBuckets = new LinkedList[newBucketCount];

        for(int i =0; i< _bucketCount;i++){
            Node *tmp = _buckets[i].get_head();
            while (tmp != NULL)
            {
                newBucketIndex= hashFunction(tmp->key) % newBucketCount;
                newBuckets[newBucketIndex].add_node(tmp->key, tmp->value);
                tmp = tmp->next;
            }
        }
        newBucketIndex= hashFunction(key) % newBucketCount;
        newBuckets[newBucketIndex].add_node(key,value);
        delete[] _buckets; //clear memory
        _buckets = newBuckets;
        _itemsCount ++;
        _bucketCount = newBucketCount;
        _loadFactor = (double)_itemsCount/(double)newBucketCount;

     } else {

        _buckets[bucketIndex].add_node(key,value);
         _itemsCount ++;
         _loadFactor = loadFactor;
     }

    }


}

bool HashMap::remove(const std::string& key){
    if(key.empty()) return false;

    unsigned int bucketIndex = hashFunction(key) % _bucketCount;
    unsigned int result = _buckets[bucketIndex].delete_node(key);
    if(result){
        _itemsCount--;
        _loadFactor = (double)_itemsCount/(double)_bucketCount;
    }
    return result;
    
}

bool HashMap::contains(const std::string& key) const{
    if(key.empty()) return false;
    unsigned int bucketIndex = hashFunction(key) % _bucketCount;
    std::pair<Node, bool> result = _buckets[bucketIndex].get_node(key);
    return result.second;
}

std::string HashMap::value(const std::string& key) const{
    if(key.empty()) return "";
    unsigned int bucketIndex = hashFunction(key) % _bucketCount;
    std::pair<Node, bool> result = _buckets[bucketIndex].get_node(key);
    if(result.second){
        return result.first.value;
    }
    return "";
}

double HashMap::loadFactor() const{
    return _loadFactor;
}

unsigned int HashMap::bucketCount() const{
    return _bucketCount;
}

unsigned int HashMap::size() const {
    return _itemsCount;
}

unsigned int HashMap::maxBucketSize() const{
 unsigned int result = 0;
 for(int i =0; i< _bucketCount;i++){
     result = std::max(result, _buckets[i].size());
 }
 return result;
}
