#include <iostream>

template <typename T>
class A {
public:
	A(T var) : var(var) {}
	void Print(T adder) {
		std::cout << var + adder << std::endl;
	}
private:
	T var;
};

template <typename> class B;
template <template<typename> class Class, typename T>
class B<Class<T>> {
public:
	B(Class<T> var) : var(var) {}
	void Print(T adder) {
		var.Print(adder);
	}
private:
	Class<T> var;
};

B<A<size_t>> global(42);

int main() {
	global.Print(7);
}
