#include <HashMap.h>

#include <cstdint>
#include <string>
#include <map>

#include <boost/test/unit_test.hpp>

#include <boost/mpl/list.hpp>

using TestedKeyTypes = boost::mpl::list<std::int32_t, std::uint64_t>;

template <typename K>
using Map = aisdi::HashMap<K, std::string>;

using std::begin;
using std::end;

BOOST_AUTO_TEST_SUITE(HashMapsTests)

template <typename K>
void thenMapContainsItems(const Map<K>& map,
                          const std::map<K, std::string>& expected)
{
  BOOST_CHECK_EQUAL(map.getSize(), expected.size());

  for (const auto& item : expected)
  {
    const auto it = map.find(item.first);
    BOOST_REQUIRE_MESSAGE(it != end(map), "Missing required item with key: " << item.first);
    BOOST_CHECK_MESSAGE(it->second == item.second,
                        "Wrong value in map for key: " << item.first
                        << " (expected: \"" << item.second
                        << "\" got: \"" << it->second << "\")");
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenMap_WhenCreatedWithDefaultConstructor_ThenItIsEmpty,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;

  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenAddingItem_ThenItIsNoLongerEmpty,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  map[K{}] = std::string{};

  BOOST_CHECK(!map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenGettingIterators_ThenBeginEqualsEnd,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  BOOST_CHECK(begin(map) == end(map));
  BOOST_CHECK(const_cast<const Map<K>&>(map).begin() == map.end());
  BOOST_CHECK(map.cbegin() == map.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenGettingIterator_ThenBeginIsNotEnd,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[K{}] = std::string{};

  BOOST_CHECK(begin(map) != end(map));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenMapWithOnePair_WhenIterating_ThenPairIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[753] = "Rome";

  auto it = map.begin();

  BOOST_CHECK_EQUAL(it->first, 753);
  BOOST_CHECK_EQUAL(it->second, "Rome");
  BOOST_CHECK(++it == map.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPostIncrementing_ThenPreviousPositionIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[K{}] = std::string{};

  auto it = map.begin();
  auto postIncrementedIt = it++;

  BOOST_CHECK(postIncrementedIt == map.begin());
  BOOST_CHECK(it == map.end());
  BOOST_CHECK(postIncrementedIt == map.cbegin());
  BOOST_CHECK(it == map.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPreIncrementing_ThenNewPositionIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[K{}] = std::string{};

  auto it = map.begin();
  auto preIncrementedIt = ++it;

  BOOST_CHECK(preIncrementedIt == it);
  BOOST_CHECK(it == map.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenIncrementing_ThenOperationThrows,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  BOOST_CHECK_THROW(map.end()++, std::out_of_range);
  BOOST_CHECK_THROW(++(map.end()), std::out_of_range);
  BOOST_CHECK_THROW(map.cend()++, std::out_of_range);
  BOOST_CHECK_THROW(++(map.cend()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenDecrementing_ThenIteratorPointsToLastItem,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[1] = std::string{};

  auto it = map.end();
  --it;

  BOOST_CHECK(it == begin(map));
  BOOST_CHECK_EQUAL(it->first, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPreDecrementing_ThenNewIteratorValueIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[1] = std::string{};

  auto it = map.end();
  auto preDecremented = --it;

  BOOST_CHECK(it == preDecremented);
  BOOST_CHECK_EQUAL(it->first, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPostDecrementing_ThenOldIteratorValueIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[1] = std::string{};

  auto it = map.end();
  auto postDecremented = it--;

  BOOST_CHECK(postDecremented == map.end());
  BOOST_CHECK_EQUAL(it->first, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenBeginIterator_WhenDecrementing_ThenOperationThrows,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  BOOST_CHECK_THROW(map.begin()--, std::out_of_range);
  BOOST_CHECK_THROW(--(map.begin()), std::out_of_range);
  BOOST_CHECK_THROW(map.cbegin()--, std::out_of_range);
  BOOST_CHECK_THROW(--(map.cbegin()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenDereferencing_ThenOperationThrows,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  BOOST_CHECK_THROW(*map.end(), std::out_of_range);
  BOOST_CHECK_THROW(*map.cend(), std::out_of_range);
  BOOST_CHECK_THROW(map.end()->first, std::out_of_range);
  BOOST_CHECK_THROW(map.cend()->second, std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenConstIterator_WhenDereferencing_ThenItemIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[42] = "Answer";

  const auto it = map.cbegin();

  BOOST_CHECK_EQUAL(it->first, 42);
  BOOST_CHECK_EQUAL(it->second, "Answer");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenSearchingForKey_ThenEndIsReturned,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;

  const auto it = map.find(123);

  BOOST_CHECK(it == end(map));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenSearchingForMissingKey_ThenEndIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[321] = "Not it";

  const auto it = map.find(123);

  BOOST_CHECK(it == end(map));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenSearchingForKey_ThenItemIsReturned,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[321] = "Not it";
  map[123] = "It!";

  const auto it = map.find(123);

  BOOST_CHECK(it != end(map));
  BOOST_CHECK_EQUAL(it->first, 123);
  BOOST_CHECK_EQUAL(it->second, "It!");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenGettingSize_ThenZeroIsReturnd,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;

  BOOST_CHECK_EQUAL(map.getSize(), 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenGettingSize_ThenItemCountIsReturnd,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  map[1] = "1";
  map[2] = "1";

  BOOST_CHECK_EQUAL(map.getSize(), 2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenMap_WhenInitializingFromListOfPairs_ThenAllItemsAreInMap,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  thenMapContainsItems(map, { { 42, "Alice" }, { 27, "Bob" } });
}


BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenDereferencing_ThenItemCanBeChanged,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Chuck" }, { 27, "Bob" } };

  auto it = map.find(42);
  it->second = "Alice";

  thenMapContainsItems(map, { { 42, "Alice" }, { 27, "Bob" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenAddingItem_ThenItemIsInMap,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  map[42] = "Alice";

  thenMapContainsItems(map, { { 42, "Alice" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenChangingItem_ThenNewValueIsInMap,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Chuck" }, { 27, "Bob" } };

  map[42] = "Alice";

  thenMapContainsItems(map, { { 42, "Alice" }, { 27, "Bob" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenCreatingCopy_ThenBothMapsAreEmpty,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;
  const Map<K> other(map);

  BOOST_CHECK(other.isEmpty());
  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenCreatingCopy_ThenAllItemsAreCopied,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 753, "Rome" }, { 1789, "Paris" } };
  const Map<K> other{map};

  map[1410] = "Grunwald";

  thenMapContainsItems(map, { { 1410, "Grunwald" }, { 753, "Rome" }, { 1789, "Paris" } });
  thenMapContainsItems(other, { { 753, "Rome" }, { 1789, "Paris" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenMovingToOther_ThenBothMapsAreEmpty,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  Map<K> other{std::move(map)};

  BOOST_CHECK(map.isEmpty());
  BOOST_CHECK(other.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenMovingToOther_ThenAllItemsAreMoved,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 753, "Rome" }, { 1789, "Paris" } };
  const Map<K> other{std::move(map)};

  thenMapContainsItems(other, { { 753, "Rome" }, { 1789, "Paris" } });
  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenAssigningToOther_ThenOtherMapIsEmpty,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;
  Map<K> other = { { 42, "Alice" }, { 27, "Bob" } };

  other = map;

  BOOST_CHECK(other.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenAssigningToOther_ThenAllElementsAreCopied,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 753, "Rome" }, { 1789, "Paris" } };
  Map<K> other = { { 42, "Alice" }, { 27, "Bob" } };

  other = map;
  map[1410] = "Grunwald";

  thenMapContainsItems(other, { { 753, "Rome" }, { 1789, "Paris" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenSelfAssigning_ThenNothingHappens,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  map = map;

  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenSelfAssigning_ThenNothingHappens,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  map = map;

  thenMapContainsItems(map, { { 42, "Alice" }, { 27, "Bob" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenMoveAssigning_ThenBothMapsAreEmpty,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;
  Map<K> other = { { 42, "Alice" }, { 27, "Bob" } };

  other = std::move(map);

  BOOST_CHECK(other.isEmpty());
  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyMap_WhenMoveAssigning_ThenAllElementsAreMoved,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 753, "Rome" }, { 1789, "Paris" } };
  Map<K> other = { { 42, "Alice" }, { 27, "Bob" } };

  other = std::move(map);

  thenMapContainsItems(other, { { 753, "Rome" }, { 1789, "Paris" } });
  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenReadingValueOfAnyKey_ThenExceptionIsThrown,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;

  BOOST_CHECK_THROW(map.valueOf(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenReadingValueOfMissingKey_ThenExceptionIsThrown,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  BOOST_CHECK_THROW(map.valueOf(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenReadingValueOfAKey_ThenValueIsReturned,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  BOOST_CHECK_EQUAL(map.valueOf(42), "Alice");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenChangingValueOfAKey_ThenValueIsChanged,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  map.valueOf(42) = "Chuck";

  thenMapContainsItems(map, { { 42, "Chuck" }, { 27, "Bob" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyMap_WhenRemovingValueByKey_ThenExceptionIsThrown,
                              K,
                              TestedKeyTypes)
{
  Map<K> map;

  BOOST_CHECK_THROW(map.remove(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenRemovingValueByWrongKey_ThenExceptionIsThrown,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  BOOST_CHECK_THROW(map.remove(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenRemovingValueByKey_ThenItemIsRemoved,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  map.remove(27);

  thenMapContainsItems(map, { { 42, "Alice" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenSingleItemMap_WhenRemovingValueByKey_ThenMapBecomesEmpty,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 27, "Bob" } };

  map.remove(27);

  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenErasingEnd_ThenExceptionIsThrown,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  BOOST_CHECK_THROW(map.remove(end(map)), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyMap_WhenRemovingItemByIterator_ThenItemIsRemoved,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };

  map.remove(map.find(42));

  thenMapContainsItems(map, { { 27, "Bob" } });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenSingleItemMap_WhenRemovingItemByIterator_ThenMapBecomesEmpty,
                              K,
                              TestedKeyTypes)
{
  Map<K> map = { { 42, "Alice" } };

  map.remove(map.find(42));

  BOOST_CHECK(map.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenTwoEmptyMaps_WhenComparingThem_ThenTheyAreReportedAsEqual,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map;
  const Map<K> other;

  BOOST_CHECK(map == other);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenTwoEqualMaps_WhenComparingThem_ThenTheyAreReportedAsEqual,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };
  const Map<K> other = { { 42, "Alice" }, { 27, "Bob" } };

  BOOST_CHECK(map == other);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenTwoEquivalentMaps_WhenComparingThem_ThenTheyAreReportedAsEqual,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };
  const Map<K> other = { { 27, "Bob" }, { 42, "Alice" } };

  BOOST_CHECK(map == other);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenTwoMapsWithDifferentValues_WhenComparingThem_ThenTheyAreNotEqual,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" } };
  const Map<K> other = { { 27, "Alice" }, { 42, "Bob" } };

  BOOST_CHECK(map != other);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenTwoMapsWithDifferentKeys_WhenComparingThem_ThenTheyAreNotEqual,
                              K,
                              TestedKeyTypes)
{
  const Map<K> map = { { 42, "Alice" }, { 27, "Bob" }, { 13, "Chuck" } };
  const Map<K> other = { { 27, "Alice" }, { 42, "Bob" } };

  BOOST_CHECK(map != other);
}

// ConstIterator is tested via Iterator methods.
// If Iterator methods are to be changed, then new ConstIterator tests are required.

BOOST_AUTO_TEST_SUITE_END()
