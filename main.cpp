#include <iostream>
#include <memory>
#include <string>

#include "custom_allocator.h"

class Sample {
public:
	Sample(std::string &&name = "") : name(std::move(name)) {}
	~Sample() { std::cout << "Destructing " << name << std::endl; }
private:
	std::string name;
};

template <typename Del = std::default_delete<Sample>> using pSample = std::unique_ptr<Sample, Del>;
static CustomHeap<Sample, 100> heap;

int main() {
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	{
		pSample<Deleter<heap>> ptr2(new(heap) Sample("Sample 2"));
		{
			pSample<Deleter<heap>> ptr1(new(heap) Sample("Sample 1"));
			std::cout << "Size of sample 1: " << sizeof(ptr2) << std::endl;
			std::cout << "Free space: " << heap.FreeSpace() << std::endl;
		}
		std::cout << "Size of sample 2: " << sizeof(ptr2) << std::endl;
		std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	}
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;

	system("pause");
}
