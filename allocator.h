#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <type_traits>

template <typename Type>
class Allocator {
public:
	using _AllocElementType = Type;
	virtual Type *Allocate() = 0;
	virtual void Free(Type *ptr) = 0;
	virtual ~Allocator() = default;
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

#endif
