#include <iostream>

template <typename Member, typename Base>
class MemberBasePair : private Base {
public:
	MemberBasePair(Member member, Base &&base = Base()) : Base(std::move(base)), mem(member) {}
	Base &base() { return *this; }
	const Base &base() const { return *this; }
	Member &member() { return mem; }
	const Member &member() const { return mem; }
private:
	Member mem;
};

class Empty {
public:
	static int factorial(int n);
};

int main() {
	MemberBasePair<size_t, Empty> pair(42);

	std::cout << sizeof(pair) << std::endl;
	std::cout << pair.member() << std::endl;
	std::cout << pair.base().factorial(7) << std::endl;

	system("pause");
}

int Empty::factorial(int n) {
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}