#include <array>
#include <iostream>
#include <string>

template <typename Type, size_t Size>
class CustomHeap;

template <typename Type, size_t Size, CustomHeap<Type, Size> &Heap>
class DeleterTemplate {
public:
	void operator()(Type *ptr) {
		std::cout << "In unallocate" << std::endl;
		Heap.Free(ptr);
	};
};

template <typename Type, size_t Size>
class CustomHeap {
public:
	template <CustomHeap<Type, Size> &Heap>
	using Deleter = DeleterTemplate<Type, Size, Heap>;

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
private:
	std::string name;
};

static CustomHeap<Sample, 100> heap;

int main() {
	std::cout << "Free space: " << heap.FreeSpace() << std::endl;
	{
		std::unique_ptr<Sample, decltype(heap)::Deleter<heap>> ptr2(heap.Allocate());
		{
			std::unique_ptr<Sample, decltype(heap)::Deleter<heap>> ptr1(heap.Allocate());
			std::cout << "Size of sample 1: " << sizeof(ptr2) << std::endl;
			std::cout << "Free space: " << heap.FreeSpace() << std::endl;
		}
		std::cout << "Size of sample 2: " << sizeof(ptr2) << std::endl;
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
