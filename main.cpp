#include <iostream>
#include <memory>
#include <string>

#include "custom_allocator.h"

class Wrapper {
public:
	Wrapper(std::string &&name) : name(std::move(name)) { std::cout << "Constructing name" << std::endl; }
	~Wrapper() { std::cout << "Destructing name" << std::endl; }
	friend std::ostream &operator<<(std::ostream &os, const Wrapper &wrapper);
private:
	std::string name;
};
std::ostream &operator<<(std::ostream &os, const Wrapper &wrapper) {
	return os << wrapper.name;
}

class Sample {
public:
	Sample(std::string &&name = "") : name(std::move(name)) {
		std::cout << "Constructing " << this->name << std::endl;
		if (count++ == 0)
			throw std::exception();
	}
	~Sample() { std::cout << "Destructing " << name << std::endl; }
private:
	static size_t count;
	Wrapper name;
};
size_t Sample::count = 0;

template <typename Del = std::default_delete<Sample>> using pSample = std::unique_ptr<Sample, Del>;
static CustomHeap<Sample, 100> heap;

int main() {
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	{
		try {
			pSample<Deleter<heap>> ptr3(new(heap) Sample("Sample 3"));
		}
		catch (std::exception e) {
			std::cout << "Caught Exception!" << std::endl;
		}
		std::cout << "Free space: " << heap.FreeSpace() << std::endl;
		pSample<Deleter<heap>> ptr2(new(heap) Sample("Sample 2"));
		{
			pSample<Deleter<heap>> ptr1(new(heap) Sample("Sample 1"));
			std::cout << "Free space: " << heap.FreeSpace() << std::endl;
		}
		std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	}
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;

	system("pause");
}
