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
	void addPrint(typename A<T>::Type adder) const { std::cout << var + adder << std::endl; }
private:
	typename A<T>::Type var;
};

template <typename T>
void Printer(const T &print, typename T::Type adder) {
	print.addPrint(adder);
}

int main() {
	B<size_t> b(43);
	Printer(b, 4);
	system("pause");
}
