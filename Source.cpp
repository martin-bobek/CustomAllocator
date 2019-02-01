#include <iostream>

template <typename T>
class A {
public:
	typedef T Type;
};

template <typename T>
class B : public A<T> {
public:
	B(typename A<T>::Type var) : var(var) {}
	typename A<T>::Type var;
};

int main() {
	B<size_t> b(43);
	std::cout << b.var << std::endl;
	system("pause");
}
