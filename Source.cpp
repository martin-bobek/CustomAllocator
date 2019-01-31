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

template <auto &> class C;
template <template<typename> class Middle, typename T, template<typename> class Class, Class<Middle<T>> &ptr>
class C<ptr> {
public:
	void Print(Class<Middle<T>> &obj, T adder) {
		obj.Print(adder);
	}
};


B<A<size_t>> global(42);	

int main() {
	C<global> printer;
	printer.Print(global, 7);
}
