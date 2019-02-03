#ifndef STATIC_VECTOR_H_
#define STATIC_VECTOR_H_

#include <new>
#include <type_traits>

template <typename Type, size_t Size>
class StaticVector {
public:
	Type &operator[](size_t index) noexcept { return *std::launder(reinterpret_cast<Type*>(&vector[index])); }
	const Type &operator[](size_t index) const noexcept { return *std::launder(reinterpret_cast<const Type*>(&vector[index])); }
private:
	std::array<std::aligned_storage_t<sizeof(Type), alignof(Type)>, Size> vector;
};

#endif
