#include <iostream>

template <typename T>
class Referenced {
public:
	void PrintT(T var) {
		std::cout << "Hello there: " << var << std::endl;
	}
};

template <typename T, Referenced<T> &ref>
class Reference {
public:
	void Trigger(T var) {
		ref.PrintT(var);
	}
};

Referenced<size_t> target;

int main() {
	Reference<size_t, target> ref;
	ref.Trigger(42);
	system("pause");
}
