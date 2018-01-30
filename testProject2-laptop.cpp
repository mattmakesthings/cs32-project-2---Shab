#include "Map-laptop.h"
#include <iostream>
using namespace std;

int main()
{
	Map m1, m2, result;
	std::string key;
	double value;

	m1.insert("Dom", 234);
	m1.insert("Sal", 159);
	m1.insert("Mel", 312);

	m2.insert("Yaz", 340);
	m2.insert("Sal", 159);
	m2.insert("Mel", 140);

	cout << "m1:" << endl;
	m1.dump();
	cout << endl;

	cout << "m2:" << endl;
	m2.dump();
	cout << endl;

	combine(m1, m2, result);
	cout << "m1 combined with m2:" << endl;
	result.dump();
	cout << endl;

	subtract(m1, m2, result);
	cout << "m2 subtracted from m1:" << endl;
	result.dump();
	cout << endl;
}