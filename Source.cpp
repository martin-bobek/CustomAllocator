#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

template <typename Type>
class Allocator {
public:
	using _AllocElementType = Type;
	virtual Type *Allocate() = 0;
	virtual void Free(Type *ptr) = 0;
	virtual ~Allocator() = default;
};

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

template <auto &rHeap,
	std::enable_if_t<std::is_convertible_v<std::remove_reference_t<decltype(rHeap)> *,
	Allocator<typename std::remove_reference_t<decltype(rHeap)>::_AllocElementType> *>> * = nullptr>
struct Deleter {
	using Type = typename std::remove_reference_t<decltype(rHeap)>::_AllocElementType;
	void operator()(Type *ptr) const { rHeap.Free(ptr); };
};

template <typename Heap, std::enable_if_t<std::is_convertible_v<Heap *, Allocator<typename Heap::_AllocElementType> *>> * = nullptr>
inline void *operator new(size_t, Heap &heap) {
	return heap.Allocate();
}

class Sample {
public:
	Sample(std::string &&name = "") : name(std::move(name)) {}
	~Sample() { std::cout << "Destructing " << name << std::endl; }
private:
	std::string name;
};

static CustomHeap<Sample, 100> heap;
template <typename Del = std::default_delete<Sample>> using pSample = std::unique_ptr<Sample, Del>;

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

template <typename Type, size_t Size> Type *CustomHeap<Type, Size>::Allocate() {
	if (!free)
		return nullptr;
	free--;

	return &heap[findFree()];
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
