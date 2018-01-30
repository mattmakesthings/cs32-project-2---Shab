// Map.cpp

#include "Map-laptop.h"
#include <iostream>
using namespace std;

Map::Map()
	: m_size(0)
{
	head = nullptr;			//initializing head to point to nothing
}

Map::Map(const Map& other)
{
	m_size = 0;
	head = nullptr;
	if (other.m_size == 0)		//in case other was an empty map
		head = nullptr;

	else
	{
		Node* p;
		int i = 0;
		//walk through other map and insert() all Nodes into the new map
		for (p = other.head; i < other.m_size; p = p->m_next, i++)
		{
			insert(p->m_pair.m_key, p->m_pair.m_value);
		}
	}
}

Map& Map::operator= (const Map& rhs)
{
	if (this != &rhs)		//to prevent aliasing problems
	{
		//first clean up the existing map
		//same code that's inside my destructor:
		Node* p;
		while (head != nullptr)
		{
			p = head;
			head = head->m_next;
			delete p;
			m_size--;
		}


		if (rhs.m_size == 0)		//in case rhs was an empty map
			;

		else
		{
			Node* q;
			int i = 0;
			//walk through rhs map and insert() all Nodes into the lhs map
			for (q = rhs.head; i < rhs.m_size; q = q->m_next, i++)
			{
				insert(q->m_pair.m_key, q->m_pair.m_value);
			}
		}

		m_size = rhs.m_size;
	}
	return *this;
}

bool Map::erase(const KeyType& key)
{
	Node* p;

	//start at head and walk through the linked list
	//until you find the key, or you go through the entire list without finding it

	for (p = head; p != nullptr && p->m_pair.m_key != key; p = p->m_next)
		;

	if (p == nullptr)
		return false;		//happens if list was empty,
							//or we went through the list without finding the key
							//because last item's m_next is nullptr

	else					//key does exist in map and p is now pointing to its corresponding Node
	{
		//in the normal case
		if (p->m_prev != nullptr && p->m_next != nullptr)
		{
			p->m_prev->m_next = p->m_next;
			p->m_next->m_prev = p->m_prev;	//make nodes before and after point to each other
		}

		//in case key is in the first Node:
		if (p->m_prev == nullptr && p->m_next != nullptr)
		{
			head = p->m_next;				//reset what head points to
			p->m_next->m_prev = p->m_prev;	//make the 2nd item's m_prev be nullptr
		}

		//in case key is in the last Node:
		if (p->m_next == nullptr && p->m_prev != nullptr)
		{
			p->m_prev->m_next = p->m_next;	//make the item before last one be the last item
		}

		//in case there's only one Node and it has the key
		if (p->m_prev == nullptr && p->m_next == nullptr)
		{
			head = nullptr;		//no more items left in the list so head points to nothing
		}

		delete p;							//pointer p is deallocated and Node is erased
		m_size--;
		return true;
	}


}

bool Map::get(const KeyType& key, ValueType& value) const
{
	Node* p;

	//start at head and walk through the linked list
	//until you find the key, or you go through the entire list without finding it

	for (p = head; p != nullptr && p->m_pair.m_key != key; p = p->m_next)
		;

	if (p == nullptr)
		return false;		//happens if list was empty,
							//or we went through the list without finding the key
							//because last item's m_next is nullptr

	else					//happens when key is found and p is now pointing to its corresponding Node
	{
		value = p->m_pair.m_value;
		return true;
	}
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	Node* p;
	int j = 0;
	if (i < 0 || i >= m_size)
		return false;

	for (p = head; j < i; p = p->m_next, j++)			//to get to the i'th Node in list
		;
	key = p->m_pair.m_key;
	value = p->m_pair.m_value;
	return true;
}

void Map::swap(Map& other)
{
	//to swap maps, we need to swap their m_size's
	//and swap what their heads point to

	Node* p;		//pointer to help prevent memory leak
	int dummy;

	p = other.head;
	other.head = head;
	head = p;			//heads are swapped

	dummy = other.m_size;
	other.m_size = m_size;
	m_size = dummy;
}

Map::~Map()
{
	//we have a pointer p point to first element of list
	//while head points to the 2nd one at each iteration
	//then delete what p points to without losing the head of list

	Node* p;
	while (head != nullptr)
	{
		p = head;
		head = head->m_next;
		delete p;
		m_size--;
	}
}

int Map::find(const KeyType& key) const
{
	Node* p;
	int i = 0;

	//start at head and walk through the linked list
	//until you find the key, or you go through the entire list without finding it

	for (p = head; p != nullptr && p->m_pair.m_key != key; p = p->m_next)
		i++;

	if (p == nullptr)
		return -1;			//happens if list was empty,
							//or we went through the list without finding the key
							//because last item's m_next is nullptr
	else
		return i;			//returning the index of Node
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
	bool mayInsert, bool mayUpdate)
{
	int i = find(key);
	if (i != -1)  // found
	{
		if (mayUpdate)
		{
			Node* p;
			int j = 0;
			for (p = head; j < i; p = p->m_next, j++)			//to get to the i'th Node in list
				;
			p->m_pair.m_value = value;
		}
		return mayUpdate;
	}
	if (!mayInsert)  // not found, and not allowed to insert
		return false;

	Node *last = new Node;		//allocate memory for a new node
	last->m_pair.m_key = key;		//set new Node's pair's values
	last->m_pair.m_value = value;

	if (head != nullptr)
	{
		Node* p;
		int j = 0;
		for (p = head; j < m_size - 1; p = p->m_next, j++)			//get to the last element of current list
			;
		p->m_next = last;			//make new Node be the last element in list
		last->m_next = nullptr;
		last->m_prev = p;
	}
	else
	{
		head = last;
		head->m_next = nullptr;
		head->m_prev = nullptr;
	}

	m_size++;

	return true;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	//first, check to see if m2 and result are the same map
	//no need to check m1 and map because assignment operator function takes care of that

	bool isdifferent = 0;
	bool isrepeated = 0;

		Map dummy = m1;		//use dummy map to prevent aliasing problems

		//go through m2, compare each key with all keys in dummy
		//if key exists in both, compare values
		//if values are different, erase key from dummy
		//else, don't do anything

		std::string key2, keyr;
		double value2, valuer;
		for (int i = 0; i < m2.size(); i++)
		{
			m2.get(i, key2, value2);
			isdifferent = 0;
			isrepeated = 0;
			for (int j = 0; j < dummy.size(); j++)
			{
				dummy.get(j, keyr, valuer);

				if (key2 == keyr)
				{
					isrepeated = 1;
					if (value2 != valuer)
						isdifferent = 1;		//same key with different values found
				}
			}
			if (isdifferent)
				dummy.erase(key2);
			if (!isrepeated)
				dummy.insert(key2, value2);
		}

		result = dummy;


	if (isdifferent)
		return false;
	else return true;

}
//for keys that are either in m1 or m2, insert them in result
//keys that are in both m1 and m2 AND have same value, insert one in result
//return true in these cases
//if keys exist in both m1 and m2 but have different values,
//don't put them in result. And return false.

void subtract(const Map& m1, const Map& m2, Map& result)
{
	//use dummy to prevent aliasing problems in case m2 is the same map as result
	Map dummy = m1;		//copy m1 into dummy map
	string key;
	double value;

	for (int i = 0; i < m2.size(); i++)
	{
		m2.get(i, key, value);
		dummy.erase(key);		//erase pairs that have a key that exists in m2
	}

	result = dummy;
}
//result must contain one copy of all the pairs in m1 whose keys don't appear in m2

void Map::dump() const
{
	for (int i = 0; i < m_size; i++)
	{
		string key;
		double value;
		get(i, key, value);
		cout << key << " " << value << endl;
	}
}