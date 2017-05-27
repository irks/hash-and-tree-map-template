#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>

#define H_SIZE 50003

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;


protected:
  unsigned minimalHash, maximalHash;
  std::list<value_type>* hashArray;
  unsigned size;

  unsigned hashFunction(const key_type key) const {
    return ((std::hash<key_type>{}(key)) % H_SIZE);
  }

  		iterator insert(const_reference entry) {
      unsigned hashIndex = hashFunction(entry.first) ;
      if(isEmpty()) {
        minimalHash = hashIndex;
        maximalHash = hashIndex;
      }
      else {
        if(minimalHash > hashIndex)
          minimalHash = hashIndex;
				if(maximalHash < hashIndex)
					maximalHash = hashIndex;
      }

      ++size;

      auto it = hashArray[hashIndex].begin();
      for(; it != hashArray[hashIndex].end(); ++it ) {
        if(it->first < entry.first)
          return iterator(this, hashIndex, (hashArray[hashIndex].insert(it, entry)));
      }
      return iterator(this, hashIndex, (hashArray[hashIndex].insert(it, entry)));
		}


public:

  HashMap() :  minimalHash(0), maximalHash(0), size(0) {
    hashArray = new std::list<value_type>[H_SIZE];
  }

  HashMap(std::initializer_list<value_type> list) : HashMap() {
    for(auto it = list.begin(); it != list.end(); ++it)
      insert(*it);
  }

  HashMap(const HashMap& other) : HashMap() {
    for(auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
  }

  HashMap(HashMap&& other) {
    size = other.size;
    minimalHash = other.minimalHash;
    maximalHash = other.maximalHash;
    hashArray = other.hashArray;

    other.size = other.minimalHash = maximalHash = 0;
    other.hashArray = new std::list<value_type>[H_SIZE];
  }

  ~HashMap() {
    delete [] hashArray;
  }

  HashMap& operator=(const HashMap& other) {
    if(this == &other)
      return *this;

    if(!isEmpty()) {
      delete [] hashArray;
      hashArray = new std::list<value_type>[H_SIZE];
      size = 0;
    }
    for(auto it = other.begin(); it != other.end(); ++it)
      insert(*it);

    return *this;
  }

  HashMap& operator=(HashMap&& other) {
    if(this == &other)
      return *this;

    std::list<value_type>* toDel = hashArray;

    size = other.size;

    minimalHash = other.minimalHash;
    maximalHash = other.maximalHash;
    hashArray = other.hashArray;
    other.size = 0;
    other.hashArray = new std::list<value_type>[H_SIZE];

    delete [] toDel;

    return *this;
  }

  bool isEmpty() const {
    return !size;
  }

  mapped_type& operator[](const key_type& key) {
    iterator tmp = find(key);
    if(tmp != end())
      return tmp->second;
    else {
      iterator it = insert(value_type(key, mapped_type()));
      return it->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const {
    if(isEmpty())
      throw std::out_of_range("valueOf in empty hash map");
    const_iterator it = find(key);
    if(it == cend())
      throw std::out_of_range("valueOf element that is not in hashmap");
    return it->second;
  }

  mapped_type& valueOf(const key_type& key) {
    if(isEmpty())
      throw std::out_of_range("valueOf in empty hash map");
    iterator it = find(key);
    if(it == end())
      throw std::out_of_range("valueOf element that is not in hashmap");
    return it->second;
  }

  const_iterator find(const key_type& key) const {
    unsigned index = hashFunction(key);
    auto it = hashArray[index].begin();
    while(it != hashArray[index].end()) {
      if(key == it->first)
        return const_iterator(this, index, it);
      ++it;
    }
    return cend();
  }

  iterator find(const key_type& key) {
    unsigned index = hashFunction(key);
    auto it = hashArray[index].begin();
    while(it != hashArray[index].end()) {
      if(key == it->first)
        return iterator(this, index, it);
      ++it;
    }
    return end();
  }

  void remove(const key_type& key) {

    if(isEmpty())
      throw std::out_of_range("Attempt to remove element from empty hash map");

    const_iterator toDel = find(key);

    if(toDel == cend())
      throw std::out_of_range("Attempt to remove element that is not in hashmap");

    remove(toDel);
  }

  void remove(const const_iterator& it) {
    if(it == cend())
      throw std::out_of_range("Attempt to remove end iterator");
    unsigned hashIndex = it.indexIt;
    hashArray[hashIndex].erase(it.iterIt);
    --size;
    if(hashArray[hashIndex].empty() == false)
      return;

    if(isEmpty())
      return;

    if(minimalHash == hashIndex) {
        while(hashArray[hashIndex].empty())
          ++hashIndex;
        minimalHash = hashIndex;
    }
    if(maximalHash == hashIndex) {
        while(hashArray[hashIndex].empty())
          --hashIndex;
        maximalHash = hashIndex;
    }
  }

  size_type getSize() const {
    return size;
  }

  bool operator==(const HashMap& other) const {

    if(size != other.size || minimalHash != other.minimalHash || maximalHash != other.maximalHash)
      return false;

    if(isEmpty() && other.isEmpty())
        return true;

    unsigned tmp = minimalHash;
    while(tmp <= maximalHash) {
      if(hashArray[tmp] != other.hashArray[tmp])
        return false;
      ++tmp;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const {
    return !(*this == other);
  }

  iterator begin() {
    if(isEmpty())
      return end();
    return iterator(this, minimalHash, hashArray[minimalHash].begin());
  }

  iterator end() {
    return iterator(this, maximalHash, hashArray[maximalHash].end());
  }

  const_iterator cbegin() const {
    if(isEmpty())
      return cend();
    return const_iterator(this, minimalHash, hashArray[minimalHash].begin());
  }

  const_iterator cend() const {
    return const_iterator(this, maximalHash, hashArray[maximalHash].end());
  }

  const_iterator begin() const {
    return cbegin();
  }

  const_iterator end() const {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

protected:
    const HashMap *parentMap;
    unsigned indexIt;
    typename std::list<value_type>::iterator iterIt;

    friend void aisdi::HashMap<KeyType, ValueType>::remove(const const_iterator&);
    friend void aisdi::HashMap<KeyType, ValueType>::remove(const key_type&);

public:

  explicit ConstIterator(const HashMap* pMap, unsigned int argIndex, typename std::list<value_type>::iterator it) :
    parentMap(pMap), indexIt(argIndex), iterIt(it) {}

  ConstIterator(const ConstIterator& other) {
    parentMap = other.parentMap;
    indexIt = other.indexIt;
    iterIt = other.iterIt;
  }

  ConstIterator& operator++() {

    if(*this == parentMap->cend())
      throw std::out_of_range("Attempt to increment end iterator");

    ++iterIt; //inkrementujemy iterator z listy

    if(iterIt == parentMap->hashArray[indexIt].cend()) { //jezeli teraz iterator wskazuje na koniec listy
      if(indexIt == parentMap->maximalHash) // jezeli znajdujemy sie w ostatniej liscie w ktorej cokolwiek jest to znaczy ze teraz zwrocimy enda
        return *this;

      ++indexIt; //przechodzimy do kolejnej listy
      while(indexIt <= parentMap->maximalHash) { // przechodzimy do kolejnych list az dojdziemy do ostatniej listy w ktorej cos jest albo ponizszy warunek bedzie spelniony
        if(parentMap->hashArray[indexIt].empty() == false) { //jezeli dana lista nie jest pusta to zatrzymujemy sie w niej
          iterIt = parentMap->hashArray[indexIt].begin(); //zmieniamy iterator listowy na poczatek listy
          return *this; // po robocie ;)
        }
        ++indexIt; //zwiekszamy index w petli
      } //while
    } // if list.end

    return *this;

  }

  ConstIterator operator++(int) {
    auto ret = *this;
    operator++();
    return ret;
  }

  ConstIterator& operator--() {
    if(*this == parentMap->cbegin())
      throw std::out_of_range("Attempt to decrement begin iterator");

    if(iterIt != parentMap->hashArray[indexIt].begin()) { //jezeli nie wskazuje na poczatek listy
      --iterIt;
      return *this;
    }
    else { //wskazuje na poczatek listy
      --indexIt;
      while(indexIt >= parentMap->minimalHash) {
        if(parentMap->hashArray[indexIt].empty() == false) {
          iterIt = --(parentMap->hashArray[indexIt].end()); //iterator listowy wskazuje teraz na element ostatni w liscie
          return *this;
        }
        --indexIt;
      }
    }
    return *this;
  }

  ConstIterator operator--(int) {
    auto ret = *this;
    operator--();
    return ret;
  }

  reference operator*() const {
    if(*this == parentMap->end())
      throw std::out_of_range("Attempt to dereference end iterator");
    return *iterIt;
  }

  pointer operator->() const {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const {
    return iterIt == other.iterIt;
  }

  bool operator!=(const ConstIterator& other) const {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator(HashMap *parentMap, unsigned int ind, typename std::list<value_type>::iterator it) : ConstIterator(parentMap, ind, it){}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++() {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int) {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--() {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int) {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const {
    return &this->operator*();
  }

  reference operator*() const {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
