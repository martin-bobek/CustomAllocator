#include <iostream>

template <typename Inner>
class Middle {
public:
	Middle(Inner var) : var(var) {}
	void Print(Inner adder) { std::cout << var + adder << std::endl; }
private:
	Inner var;
};

template <typename> class Outer;
template <template<typename> class T, typename U>
class Outer<T<U>> {
public:
	Outer(T<U> var) : var(var) {}
	void Print(U adder) { var.Print(adder); }
private:
	T<U> var;
};

template <auto &> struct Reference;
template <typename V, template<typename> class U, template<typename> class T, T<U<V>> &ref>
struct Reference<ref> {
	void operator()(V adder) const { ref.Print(adder); }
};

static Outer<Middle<size_t>> global(42);

int main() {
	Reference<global> printer;
	printer(7);
}
