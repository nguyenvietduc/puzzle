// random_shuffle example
// Credit: http://www.cplusplus.com/reference/algorithm/random_shuffle/
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>

// random generator function:
int myrandom(int i) { return std::rand()%i; }

int main() {
	std::srand(unsigned (std::time(0)));
	std::vector<int> myvector;

	// set some values
	for (int i=0; i<10; i++)
		myvector.push_back(i);

	// using built-in random generator
	std::random_shuffle(myvector.begin(), myvector.end());

	// using myrandom
	std::random_shuffle(myvector.begin(), myvector.end(), myrandom);

	// print out content:
	std::cout << "myvector contains: ";
	for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';
	return 0;
}