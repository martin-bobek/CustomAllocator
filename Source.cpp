#include <iostream>

template <typename T>
class Referenced {
public:
	void PrintT(T var) {
		std::cout << "Hello there " << var << std::endl;
	}
};

template <auto> class Reference;
template <template <typename> class Class, typename T, void (Class<T>::*MF)()> class Reference<MF> {
public:
	void Trigger(Class<T> *instance, T var) {
		(instance->*MF)(var);
	}
};

Referenced<size_t> target;

int main() {
	Reference<&Referenced<size_t>::PrintT> ref;
	ref.Trigger(&target, 42);
}
