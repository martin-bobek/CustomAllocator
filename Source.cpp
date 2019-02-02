#include <iostream>

template <typename T>
class A {
public:
	typedef T Type;
	virtual void addPrint(T adder) const = 0;
	virtual ~A() = default;
};

template <typename T>
class B : public A<T> {
public:
	B(T var) : var(var) {}
	void addPrint(T adder) const { std::cout << var + adder << std::endl; }
private:
	T var;
};

class C {
public:
	typedef size_t Type;
	void addPrint(size_t adder) const { std::cout << "Error!" << std::endl; }
};

template <typename T>
void Printer(T const &print, typename std::enable_if_t<std::is_convertible<T *, A<typename T::Type> *>::value, T>::Type adder) {
	print.addPrint(adder);
}

int main() {
	B<size_t> b(43);
	C c;
	Printer(b, 4);
//  Printer(c, 7);    // Causes type deduction failure because C does not inherit from A.
	system("pause");
}
