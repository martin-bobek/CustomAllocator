#include <array>
#include <iostream>
#include <string>

template <typename Type, typename Del = std::default_delete<Type>>
class SmartPtr : private Del {
public:
	SmartPtr(Type *ptr, Del del = Del()) : Del(std::move(del)), ptr(ptr) {}
	Del &del() { return *this; }
	const Del &del() const { return *this; }
	Type *get() { return ptr; }
	const Type *get() const { return ptr; }
	Type *operator*() { return ptr; }
	const Type *operator*() const { return ptr; }
	~SmartPtr() { (*this)(ptr); }
private:
	Type *ptr;
};

template <typename Type, size_t Size>
class CustomHeap {
public:
	CustomHeap() { allocated.fill(0); }
	Type *Allocate();
	void Free(Type *ptr);
	size_t FreeSpace() const { return free; }
private:
	size_t findFree();

	typedef uint8_t allocBlock;
	static constexpr allocBlock blockFull = std::numeric_limits<allocBlock>::max();
	static constexpr size_t blockSize = 8 * sizeof(allocBlock);

	std::array<Type, Size> heap;
	std::array<allocBlock, Size / blockSize + 1> allocated;
	size_t free = Size;
};

class Sample {
public:
	Sample(std::string &&name = "") : name(std::move(name)) {}
	~Sample() {
		std::cout << "Deleting " << name << '!' << std::endl;
	}
private:
	std::string name;
};

static CustomHeap<Sample, 100> heap;

template <typename Type>
class Deleter {
public:
	void operator()(Type *ptr) {
		std::cout << "In unallocate" << std::endl;
		heap.Free(ptr);
	};
};

int main() {
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	{
		SmartPtr<Sample, Deleter<Sample>> ptr2(heap.Allocate());
		{
			SmartPtr<Sample, Deleter<Sample>> ptr1(heap.Allocate());
			std::cout << "Size of sample 1: " << sizeof(ptr2) << std::endl;
			std::cout << "Free space: " << heap.FreeSpace() << std::endl;
		}
		std::cout << "Size of sample 2:  " << sizeof(ptr2) << std::endl;
		std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	}
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;

	system("pause");
}

template <typename Type, size_t Size> Type *CustomHeap<Type, Size>::Allocate() {
	if (!free)
		return nullptr;
	free--;

	return new(&heap[findFree()]) Type();
}
template <typename Type, size_t Size> void CustomHeap<Type, Size>::Free(Type *ptr) {
	size_t index = (ptr - &heap[0]) / sizeof(Type);
	allocated[index / blockSize] &= ~(1 << (index % blockSize));
	free++;

	ptr->~Type();
}
template <typename Type, size_t Size> size_t CustomHeap<Type, Size>::findFree() {
	size_t block = 0, bit = 0;
	while (allocated[block] == blockFull)
		block++;
	
	allocBlock mask = 1;
	while (allocated[block] & mask) {
		mask <<= 1;
		bit++;
	}

	allocated[block] |= mask;
	return 8 * block + bit;
}
