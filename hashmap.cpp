#include "hashmap.hpp"
#include <string>
#include <iostream>
#include <vector>

int main(){
    HashMap map;
     std::vector<std::string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!!!!"};

    for (const std::string& word : msg)
    {
        std::cout << word << " ";
    }

    

    map.add("abc","password1");
    std::cout << "\nSize:" << map.size() << "\n";
    map.add("abcd","password1");
    std::cout << "Size:" << map.size() << "\n";
    map.add("abc","password1");
    std::cout << "Size:" << map.size() << "\n";

    std::cout << "Password for abc:" <<map.value("abc") << "\n";
    std::cout << "Bucket Count:" <<map.bucketCount() << "\n";
    std::cout << "MaxBucketSize:" <<map.maxBucketSize() << "\n";
    std::cout << "Contains abcd:" <<map.contains("abcd") << "\n";

    map.remove("abc");
    std::cout << "Size:" << map.size() << "\n";
    std::cout << "Contains abc:" <<map.contains("abc") << "\n";

    map.add("abc","password2");
    map.add("1","password1");
    map.add("2","password1");
    map.add("3","password1");
    map.add("4","password1");
    map.add("5","password1");
    map.add("6","password1");
    map.add("7","password1");

    std::cout << "Size:" << map.size() << "\n";
    std::cout << "Bucket Count:" <<map.bucketCount() << "\n";
    std::cout << "MaxBucketSize:" <<map.maxBucketSize() << "\n";
    std::cout << "Load Factor:" <<map.loadFactor() << "\n";
    std::cout << std::endl;
}

HashMap::HashMap()
{
    hashFunction = [](const std::string &key) {
         unsigned int fnvPrime = 1099511628211;
            unsigned int hashVal = 14695981039346656037;
            int len;
            
            for (int i = 0; i < key.length(); ++i)
            {
                hashVal ^= key[i];
                hashVal *= fnvPrime;
            }

            return hashVal;

    };
}


HashMap::HashMap(HashFunction func){
    hashFunction = func;
}
HashMap:: HashMap(const HashMap& hm){

}
HashMap::~HashMap(){
delete [] _buckets;
}


void HashMap::add(const std::string& key, const std::string& value)
{
    if(key.empty() || value.empty()) return; // do nothing

    unsigned int bucketIndex = hashFunction(key) % _bucketCount;
     
    std::pair<Node, bool> result = _buckets[bucketIndex].get_node(key);

    if(!result.second){

     //add
     _loadFactor = (double)(_itemsCount + 1) /(double) _bucketCount;
     if(_loadFactor > 0.8){
      //need to rebalance the buckets
      unsigned int newBucketCount = _bucketCount * 2 + 1;
      LinkedList *newBuckets = new LinkedList[newBucketCount];

      for(int i =0; i< _bucketCount;i++){
          Node *tmp = _buckets[i].get_head();
          while (tmp != NULL)
          {
              unsigned int newBucketIndex= hashFunction(tmp->key) % newBucketCount;
              newBuckets[newBucketIndex].add_node(tmp->key, tmp->value);
              tmp = tmp->next;
          }
      }
      delete[] _buckets; //clear memory
      _buckets = newBuckets;
     _itemsCount ++;
     _bucketCount = newBucketCount;
     _loadFactor = (double)_itemsCount/(double)newBucketCount;

     } else {

        _buckets[bucketIndex].add_node(key,value);
         _itemsCount ++;
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


