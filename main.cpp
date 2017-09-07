#include "PriorityQueue.h"
#include <stdlib.h>
#include <iostream>

int comp_int(const int& a, const int& b)
{
	return a - b;
}

int comp_str(const std::string& a, const std::string& b)
{
	return a.compare(b);
}

int main()
{
	const int MAX_SIZE = 10;
	int i = MAX_SIZE;
	
	std::cout<< "\n///////////////////// test integer specialization\n";
	PriorityQueue<int, comp_int> pq(MAX_SIZE);	

	while(i--)
		pq.insert(abs(rand()) % 10000);

	while(++i < MAX_SIZE)
		std::cout << pq.pop() << "    ";



	std::cout<< "\n///////////////////// test string specialization\n";
	PriorityQueue<std::string, comp_str> pq2(MAX_SIZE);
	pq2.insert("America"), pq2.insert("is"), pq2.insert("a"), pq2.insert("highly"), pq2.insert("industrialization"), \
	pq2.insert("country"), pq2.insert("with"), pq2.insert("advanced"), pq2.insert("economic"), pq2.insert("institution");
	i = 0;
	
	while(i++ < MAX_SIZE)
		std::cout << pq2.pop().c_str() << "    ";
	getchar();
}