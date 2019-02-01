#include <iostream>

class A {
public:
	typedef size_t Type;
};

class B : public A {
public:
	B(Type var) : var(var) {}
	Type var;
};

int main() {
	B b(43);
	std::cout << b.var << std::endl;
	system("pause");
}
