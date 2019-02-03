#include <iostream>
#include <type_traits>
#include <vector>

class Convertible {
public:
	Convertible() = default;
	Convertible(int val) : val(val) {}
	Convertible(Convertible &&) = default;
	Convertible &operator=(Convertible &&) = default;
	Convertible(const Convertible &) = default;
	Convertible &operator=(const Convertible &) = default;
	void Refresh(int val) {
		this->val = val;
		vec.clear();
	}
	operator int() { return val; }
private:
	int val;
	std::vector<size_t> vec;
};

template <typename T, typename std::enable_if_t<std::is_integral_v<T>> * = nullptr>
auto add(T lhs, T rhs) {
	std::cout << "In template add" << std::endl;
	return lhs + rhs;
}

Convertible add(int lhs, Convertible &&rhs) {
	std::cout << "In optimized add" << std::endl;
	rhs.Refresh(lhs + rhs);
	return std::move(rhs);
}

int main() {
	Convertible lhs(1), rhs(6), result;
	result = add(lhs, std::move(rhs));
	auto r = add(4, 3);
	system("pause");
}
