#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
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
  struct Node {
    Node* left;
    Node* right;
    Node* parent;

    value_type data;

    Node(const value_type& d, Node* p, Node* l, Node* r) :  left(l), right(r), parent(p), data(d) {}
  };

  Node* root;
  unsigned treeSize;



  void clearTree(Node* node) {
    if(node->left)
      clearTree(node->left);
    if(node->right)
      clearTree(node->right);
    --treeSize;
    delete node;

    return;
  }

  void copyTree(TreeMap* to, Node* from) {
    to->insert(from->data);
    if(from->left)
      copyTree(to, from->left);
    if(from->right)
      copyTree(to, from->right);
    return;
  }

  iterator insert(const_reference entry) {

    Node *tmp = root;
      while(tmp) {
        if(tmp->data.first > entry.first) {
          if(!tmp->left) { //nie ma lewego dziecka
            tmp->left = new Node(entry, tmp, nullptr, nullptr);
            ++treeSize;
            return iterator(this, tmp->left);
          }
          tmp = tmp->left;
          continue; //przejscie do kolejnej petli while
        }

        if(tmp->data.first < entry.first) {
          if(!tmp->right) { // nie ma prawego dziecka
            tmp->right = new Node(entry, tmp, nullptr, nullptr);
            ++treeSize;
            return iterator(this, tmp->right);
          }
          tmp = tmp->right;
          continue; //przejscie do kolejnej petli while
        }
        //drzewo zawiera juz element o danym kluczu
        return iterator(this, tmp);

      }

      //drzewo jest puste
      ++treeSize;
      root = new Node(entry, nullptr, nullptr, nullptr);
      return iterator(this, root);
  }


  Node* mostLeft() const {
        if(isEmpty())
        return nullptr;
        Node* tmp=root;
        while(tmp->left)
            tmp=tmp->left;
        return tmp;
  }

  Node* mostRight() const {
  if(isEmpty())
        return nullptr;
        Node* tmp=root;
        while(tmp->right)
            tmp=tmp->right;
        return tmp;
  }


public:

  TreeMap() : root(nullptr), treeSize(0) {}

  TreeMap(std::initializer_list<value_type> list) : TreeMap() {
    for(auto it = list.begin(); it!= list.end(); ++it) {
      insert(*it);
    }
  }

  TreeMap(const TreeMap& other) : TreeMap() {
    if(!other.isEmpty())
      copyTree(this, other.root);
  }

  TreeMap(TreeMap&& other) {
    root = other.root;
    treeSize = other.treeSize;

    other.treeSize = 0;
    other.root = nullptr;
  }

  TreeMap& operator=(const TreeMap& other) {
    if(this == &other)
      return *this;

    if(!isEmpty()) {
      clearTree(root);
      root = nullptr;
      treeSize = 0;
    }

    if(!other.isEmpty())
      copyTree(this, other.root);

    return *this;
  }

  TreeMap& operator=(TreeMap&& other) {
    if(this == &other)
      return *this;

    clearTree(root);

    root = other.root;
    treeSize = other.treeSize;

    other.treeSize = 0;
    other.root = nullptr;

    return *this;
  }

  ~TreeMap() {
    if(!isEmpty())
      clearTree(root);
  }

  bool isEmpty() const {
    return !root;
  }

  mapped_type& operator[](const key_type& key) {
    iterator tmp = find(key);
    if(tmp != end())
      return tmp->second;
    else {
      iterator it = insert(value_type(key, std::string{}));
      return it->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const {
    if(isEmpty())
      throw std::out_of_range("valueOf in empty tree map");
    const_iterator current = find(key);
    if(current == cend())
      throw std::out_of_range("ValueOf not existing element");
    return current->second;
  }

  mapped_type& valueOf(const key_type& key) {
    if(isEmpty())
      throw std::out_of_range("valueOf in empty tree map");
    iterator current = find(key);
    if(current == end())
      throw std::out_of_range("ValueOf not existing element");
    return current->second;
  }

  const_iterator find(const key_type& key) const {
    Node *tmp = root;
    while(tmp) {
      if(tmp->data.first > key)
        tmp = tmp->left;
      else if(tmp->data.first < key)
        tmp = tmp->right;
      else //znaleziono
        return const_iterator(this, tmp);
    }
    //nie znaleziono
    return cend();
  }

  iterator find(const key_type& key) {
    Node *tmp = root;
    while(tmp) {
      if(tmp->data.first > key)
        tmp = tmp->left;
      else if(tmp->data.first < key)
        tmp = tmp->right;
      else //znaleziono
        return iterator(this, tmp);
    }
    //nie znaleziono
    return end();
  }

  void remove(const key_type& key) {
    if(isEmpty())
      throw std::out_of_range("Attempt to remove element from empty tree map");

    const_iterator toDelIt = find(key);

    if(toDelIt == end())
      throw std::out_of_range("Remove element, which is not in tree");

    remove(toDelIt);
  }

  void remove(const const_iterator& it) {
    if(it == end())
      throw std::out_of_range("Attempt to remove end iterator");

    Node *toDel = it.node;

    if(toDel->left == nullptr && toDel->right == nullptr) { //nie ma dzieci
      if(toDel->parent != nullptr) { //jesli ma rodzica
        if(toDel->parent->left == toDel)  //jezeli jest lewym dzieckiem
          toDel->parent->left = nullptr;
        else  //jezeli jest prawym dzieckiem
          toDel->parent->right = nullptr;
      }
      else //jezeli nie ma rodzica, to znaczy ze usuwamy roota
        root = nullptr;
    }
    else if(toDel->left == nullptr) { // ma prawe dziecko
      if(toDel->parent != nullptr) { //jezeli ma rodzica
        if(toDel->parent->left == toDel)  //jezeli jest lewym dzieckiem
          toDel->parent->left = toDel->right;
        else //jezeli jest prawym dzieckiem
          toDel->parent->right = toDel->right;
      }
      else //jezeli nie ma rodzica, to znaczy ze usuwamy roota
        root = toDel->right;

      toDel->right->parent = toDel->parent;
    }
    else if(toDel->right == nullptr) { // ma lewe dziecko
      if(toDel->parent != nullptr) { //jezeli ma rodzica
        if(toDel->parent->left == toDel) //jezeli jest lewym dzieckiem
          toDel->parent->left = toDel->left;
        else  //jezeli jest prawym dzieckiem
          toDel->parent->right = toDel->left;
      }
      else //jezeli nie ma rodzica, czyli usuwamy roota
        root = toDel->left;

      toDel->left->parent = toDel->parent;
    }
    else { // ma dwoje dzieci
      Node* succ = toDel;
      succ = succ->right;
      while(succ->left != nullptr) //przejscie do najmniejszego elementu wiekszego od elementu do usuniecia
        succ = succ->left;
      //przepinanie wezlow
      if(succ != toDel->right) {
        succ->parent->left = succ->right;

        if(succ->right)
          succ->right->parent = succ->parent;

        succ->right = toDel->right;
        succ->right->parent = succ;
      }

      succ->left = toDel->left;
      succ->left->parent = succ;

      succ->parent = toDel->parent;

      if(toDel->parent != nullptr) { //ma rodzica
        if(toDel->parent->left == toDel) //jest lewym dzieckiem
          toDel->parent->left = succ;
        else //jest prawym dzieckiem
          toDel->parent->right = succ;
      }
      else //usuwamy roota
        root = succ;
    }

    --treeSize;
    delete toDel;
  }

  size_type getSize() const {
    return treeSize;
  }

  bool operator==(const TreeMap& other) const {

    if(treeSize != other.treeSize)
      return false;

    if(isEmpty() && other.isEmpty())
      return true;

    ConstIterator it = cbegin();
    ConstIterator ot = other.cbegin();

    for(; ot != other.cend(); ++ot, ++it) {
      if(*it != *ot)
        return false;
    }
    return true;
  }

  bool operator!=(const TreeMap& other) const {
    return !(*this == other);
  }

  iterator begin() {
    if(isEmpty())
      return end();
    return iterator(this, mostLeft());
  }

  iterator end() {
    return iterator(this, nullptr);
  }

  const_iterator cbegin() const {
    if(isEmpty())
      return cend();
    return const_iterator(this, mostLeft());
  }

  const_iterator cend() const {
    return const_iterator(this, nullptr);
  }

  const_iterator begin() const {
    return cbegin();
  }

  const_iterator end() const {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

protected:

  const TreeMap *tree;
  Node* node;


  friend void aisdi::TreeMap<KeyType, ValueType>::remove(const const_iterator& );

public:

  explicit ConstIterator(const TreeMap *tree, Node* ptr) : tree(tree), node(ptr) {}

  ConstIterator(const ConstIterator& other) {
    tree = other.tree;
    node = other.node;
  }

  ConstIterator& operator++() {
    if(node == nullptr)
      throw std::out_of_range("Attempt to increment end iterator");

    if(node == tree->mostRight()) {
      node = nullptr; //teraz jest to iterator end
      return *this;
    }

    if(node->right) { //jesli nie jestesmy w lisciu
      node = node->right;
      while(node->left) //szukamy nastepnika
        node = node->left;
      return *this;
    }

    if(node->parent->left == node)  //jesli jestemy w lewym dziecku
      node = node->parent;
    else { //jestesmy w prawym dziecku
      while(node->parent->right == node)
        node = node->parent;
      node = node->parent;
    }

    return *this;
  }

  ConstIterator operator++(int) {
    auto ret = *this;
    operator++();
    return ret;
  }

  ConstIterator& operator--() {
    if(node == tree->mostLeft())
      throw std::out_of_range("Attempt to decrement begin iterator");

    if(node == nullptr) { //end
      node = tree->mostRight();
      return *this;
    }

    if(node->left) { //jesli nie jestesmy w lisciu
      node = node->left;
      while(node->right) //szukamy poprzednika
        node = node->right;
      return *this;
    }

    if(node->parent->right == node) //jesli jestesmy w prawym lisciu
      node = node->parent;
    else { //jesli jestesmy w lewym lisciu
      while(node->parent->left == node)
        node = node->parent;
      node = node->parent;
    }
    return *this;
  }

  ConstIterator operator--(int) {
    auto ret = *this;
    operator--();
    return ret;
  }

  reference operator*() const {
    if(node == nullptr)
      throw std::out_of_range("attempt to dereference end iterator");
    return node->data;
  }

  pointer operator->() const {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const {
    return node == other.node;
  }

  bool operator!=(const ConstIterator& other) const {
    return !(*this == other);
  }

};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(TreeMap *tree, Node *node) : ConstIterator(tree, node) {}

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

#endif /* AISDI_MAPS_MAP_H */
