#include <iostream>

template <typename Type, typename Del>
class SmartPtr : private Del {
public:
	SmartPtr(Type *ptr, Del &&del = Del()) : Del(std::move(del)), ptr(ptr) {}
	Del &del() { return *this; }
	const Del &del() const { return *this; }
	Type *get() { return ptr; }
	const Type *get() const { return ptr; }
private:
	Type *ptr;
};

class Empty {
public:
	static int factorial(int n);
};

int main() {
	SmartPtr<size_t, Empty> ptr(new size_t(0));

	std::cout << sizeof(ptr) << std::endl;
	std::cout << *ptr.get() << std::endl;
	std::cout << ptr.del().factorial(7) << std::endl;

	system("pause");
}

int Empty::factorial(int n) {
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}