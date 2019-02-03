#ifndef CUSTOM_ALLOCATOR_H_
#define CUSTOM_ALLOCATOR_H_

#include <array>
#include <new>
#include <type_traits>

#include "allocator.h"

template <typename Type, size_t Size>
class StaticVector {
public:
	Type &operator[](size_t index) { return *std::launder(reinterpret_cast<Type *>(&vector[index])); }
	const Type &operator[](size_t index) const { return *std::launder(reinterpret_cast<const Type *>(&vector[index])); }
private:
	std::array<std::aligned_storage_t<sizeof(Type), alignof(Type)>, Size> vector;
};

template <typename Type, size_t Size>
class CustomHeap : public Allocator<Type> {
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

	StaticVector<Type, Size> heap;
	std::array<allocBlock, Size / blockSize + 1> allocated;
	size_t free = Size;
};


template <typename Type, size_t Size> inline Type *CustomHeap<Type, Size>::Allocate() {
	if (!free)
		return nullptr;
	free--;

	return &heap[findFree()];
}
template <typename Type, size_t Size> inline void CustomHeap<Type, Size>::Free(Type *ptr) {
	size_t index = (ptr - &heap[0]) / sizeof(Type);
	allocated[index / blockSize] &= ~(1 << (index % blockSize));
	free++;

	ptr->~Type();
}
template <typename Type, size_t Size> inline size_t CustomHeap<Type, Size>::findFree() {
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

#endif
