#include <iostream>
#include <string>

template <typename Type, typename Del>
class SmartPtr : private Del {
public:
	SmartPtr(Type *ptr, Del &&del = Del()) : Del(std::move(del)), ptr(ptr) {}
	Del &del() { return *this; }
	const Del &del() const { return *this; }
	Type *get() { return ptr; }
	const Type *get() const { return ptr; }
	Type *operator*() { return ptr; }
	const Type *operator*() const { return ptr; }
	~SmartPtr() { this->unallocate(ptr); }
private:
	Type *ptr;
};

template <typename Type>
class Empty {
public:
	static void unallocate(Type *ptr) {
		ptr->~Type();
		std::cout << "In unallocate" << std::endl;
	};
};

class Sample {
public:
	Sample(std::string &&name) : name(std::move(name)) {}
	~Sample() {
		std::cout << "Deleting " << name << '!' << std::endl;
	}
private:
	std::string name;
};

int main() {
	{
		SmartPtr<Sample, Empty<Sample>> ptr2(new Sample("sample 2"));
		{
			SmartPtr<Sample, Empty<Sample>> ptr1(new Sample("sample 1"));
		}
		std::cout << sizeof(ptr2) << std::endl;
	}

	system("pause");
}