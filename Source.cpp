#include <iostream>

//template <typename T>
class Referenced {
public:
	void PrintT() {
		std::cout << "Hello there" << std::endl;
	}
};

template <Referenced &ref>
class Reference {
public:
	void Trigger() {
		ref.PrintT();
	}
};

Referenced target;

int main() {
	Reference<target> ref;
	ref.Trigger();
	system("pause");
}
