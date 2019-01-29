#include <iostream>

class Empty1 {
public:
	static int factorial(int n);
};

class Empty2 : private Empty1 {};

class Empty3 : public Empty1, private Empty2 {};

int main() {
	Empty3 empty;
	std::cout << sizeof(Empty1) << std::endl;
	std::cout << sizeof(Empty2) << std::endl;
	std::cout << sizeof(Empty3) << std::endl;
	std::cout << sizeof(empty) << std::endl;
	std::cout << empty.factorial(5) << std::endl;
	system("pause");
}

int Empty1::factorial(int n) {
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}