#ifndef CUSTOM_ALLOCATOR_H_
#define CUSTOM_ALLOCATOR_H_

#include <array>
#include <type_traits>

#include "allocator.h"
#include "static_vector.h"

template <typename Type, size_t Size>
class BasicAlloc : public Allocator<Type> {
public:
	BasicAlloc() { allocated.fill(0); }
	Type *Allocate() noexcept;
	void Free(Type *ptr) noexcept;
	size_t FreeSpace() const noexcept { return free; }
private:
	size_t findFree() noexcept;

	typedef uint8_t allocBlock;
	static constexpr allocBlock blockFull = std::numeric_limits<allocBlock>::max();
	static constexpr size_t blockSize = 8 * sizeof(allocBlock);

	StaticVector<Type, Size> heap;
	std::array<allocBlock, Size / blockSize + 1> allocated;
	size_t free = Size;
};


template <typename Type, size_t Size> inline Type *BasicAlloc<Type, Size>::Allocate() noexcept {
	if (!free)
		return nullptr;
	free--;

	return &heap[findFree()];
}
template <typename Type, size_t Size> inline void BasicAlloc<Type, Size>::Free(Type *ptr) noexcept {
	size_t index = (ptr - &heap[0]) / sizeof(Type);
	allocated[index / blockSize] &= ~(1 << (index % blockSize));
	free++;
}
template <typename Type, size_t Size> inline size_t BasicAlloc<Type, Size>::findFree() noexcept {
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
