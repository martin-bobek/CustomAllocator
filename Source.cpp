#include <iostream>

template <typename T>
class A {
public:
	A(T var) : var(var) {}
	void Print() {
		std::cout << var << std::endl;
	}
private:
	T var;
};

template <typename T>
class B {
public:
	B(T var) : var(var) {}
	void Print() {
		var.Print();
	}
private:
	T var;
};

B<A<size_t>> global(42);

int main() {
	global.Print();
}
