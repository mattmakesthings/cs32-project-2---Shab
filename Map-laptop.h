// Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
	Map();               // Create an empty map (i.e., one with no key/value pairs)

	Map(const Map& other);		//copy constructor

	Map& operator= (const Map& rhs);		//assignment operator

	bool empty() const;  // Return true if the map is empty, otherwise false.

	int size() const;    // Return the number of key/value pairs in the map.

	bool insert(const KeyType& key, const ValueType& value);
	// If key is not equal to any key currently in the map, and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that either the key is already in the map, or the map has a fixed
	// capacity and is full.

	bool update(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// Otherwise, make no change to the map and return false.

	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the map, then make that key no
	// longer map to the value it currently maps to, but instead map to
	// the value of the second parameter; return true in this case.
	// If key is not equal to any key currently in the map, and if the
	// key/value pair can be added to the map, then do so and return true.
	// Otherwise, make no change to the map and return false (indicating
	// that the key is not already in the map and the map has a fixed
	// capacity and is full.

	bool erase(const KeyType& key);
	// If key is equal to a key currently in the map, remove the key/value
	// pair with that key from the map and return true.  Otherwise, make
	// no change to the map and return false.

	bool contains(const KeyType& key) const;
	// Return true if key is equal to a key currently in the map, otherwise
	// false.

	bool get(const KeyType& key, ValueType& value) const;
	// If key is equal to a key currently in the map, set value to the
	// value in the map that that key maps to and return true.  Otherwise,
	// make no change to the value parameter of this function and return
	// false.

	bool get(int i, KeyType& key, ValueType& value) const;
	// If 0 <= i < size(), copy into the key and value parameters the
	// key and value of one of the key/value pairs in the map and return
	// true.  Otherwise, leave the key and value parameters unchanged and
	// return false.

	void swap(Map& other);
	// Exchange the contents of this map with the other one.

	void dump() const;

	~Map();

private:

	// Since this structure is used only by the implementation of the
	// Map class, we'll make it private to Map.

	struct Pair
	{
		KeyType   m_key;
		ValueType m_value;
	};

	// struct for our linked list; private since it's only used by this class
	struct Node
	{
		Pair m_pair;
		Node* m_next;
		Node* m_prev;
	};

	int  m_size;                     // number of pairs in the map

	Node* head;				 //head of doubly linked list which is a pointer to the first pair
	Node dummy;				//making a dummy first Node so we'll always have a head and a tail

	int find(const KeyType& key) const;
	// Return index of the pair in m_data whose m_key == key if there is
	// one, else -1

	bool doInsertOrUpdate(const KeyType& key, const ValueType& value,
		bool mayInsert, bool mayUpdate);
	// If the key is not present in the map and if mayInsert is true, insert
	// the pair if there is room.  If the key is present and mayUpdate is
	// true, update the pair with the given key.
};

// Inline implementations

inline
int Map::size() const
{
	return m_size;
}

inline
bool Map::empty() const
{
	return size() == 0;
}

inline
bool Map::contains(const KeyType& key) const
{
	return find(key) != -1;
}

inline
bool Map::insert(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, true /* insert */, false /* no update */);
}

inline
bool Map::update(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, false /* no insert */, true /* update */);
}

inline
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	return doInsertOrUpdate(key, value, true /* insert */, true /* update */);
}

//map algorithms:

bool combine(const Map& m1, const Map& m2, Map& result);
//for keys that are either in m1 or m2, insert them in result
//keys that are in both m1 and m2 AND have same value, insert one in result
//return true in these cases
//if keys exist in both m1 and m2 but have different values,
//don't put them in result. And return false.

void subtract(const Map& m1, const Map& m2, Map& result);
//result must contain one copy of all the pairs in m1 whose keys don't appear in m2

#endif // MAP_INCLUDED
