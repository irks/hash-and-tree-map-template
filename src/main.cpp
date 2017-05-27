#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{

template <typename K, typename V>

using HashMap = aisdi::HashMap<K, V>;

template <typename K, typename V>
using TreeMap = aisdi::TreeMap<K, V>;

void perfomTest()
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> timeDifference;

  HashMap<int, std::string> hashMap;
  TreeMap<int, std::string> treeMap;
  const int size_n = 100000;
  int elementsToInsert[size_n];
  int elementsToRemove[size_n];

  for(int i = 0; i<size_n; ++i)
    elementsToInsert[i] = i;

  for(int i = 0; i<size_n; ++i)
    elementsToRemove[i] = i;

  std::random_shuffle(std::begin(elementsToInsert), std::end(elementsToInsert));
  std::random_shuffle(std::begin(elementsToRemove), std::end(elementsToRemove));

//  for(int i : elementsToInsert)
//    std::cout << " " << i;
//  std::cout  << std::endl;
//  for(int r : elementsToRemove)
//    std::cout << " " << r;
//  std::cout  << std::endl;
  //==================================================================
  //        HASHMAP
  //==================================================================

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    hashMap[elementsToInsert[i]] = "Test operator []";
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "HashMap: adding " << size_n << " elements:   " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    hashMap.find(elementsToRemove[i]);
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "HashMap: find " << size_n << " elements:     " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    hashMap.remove(elementsToRemove[i]);
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "HashMap: remove " << size_n << " elements:   " << timeDifference.count() << std::endl << std::endl;

  //====================================================================
  //        TREEMAP
  //====================================================================

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    treeMap[elementsToInsert[i]] = "Test operator []";
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "TreeMap: adding " << size_n << " elements:   " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    treeMap.find(elementsToRemove[i]);
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "TreeMap: find " << size_n << " elements:     " << timeDifference.count() << std::endl;

  start = std::chrono::system_clock::now();
  for(int i = 0; i < size_n; ++i)
    treeMap.remove(elementsToRemove[i]);
  end = std::chrono::system_clock::now();
  timeDifference = end - start;
  std::cout << "TreeMap: remove " << size_n << " elements:   " << timeDifference.count() << std::endl << std::endl;
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
