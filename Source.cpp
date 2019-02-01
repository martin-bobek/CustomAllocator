#include <map>
#include <string>

template <typename T, template<typename T, typename... Args> class C, typename... Args>
C<T, Args...> foo() {
	C<T, Args...> x;
	return x;
}

int main(void) {
	std::map<std::string, int> a = foo<std::string, std::map, int>();
}
