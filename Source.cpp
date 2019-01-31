#include <iostream>

class Referenced {
public:
	void PrintT() {
		std::cout << "Hello there" << std::endl;
	}
};

template <auto> struct Reference;
template <typename Class, void (Class::*MF)()> struct Reference<MF> {
public:
	void Trigger(Class *instance) {
		(instance->*MF)();
	}
};

Referenced target;

int main() {
	Reference<&Referenced::PrintT> ref;
	ref.Trigger(&target);
}
