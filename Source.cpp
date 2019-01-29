#include <iostream>
#include <string>

template <typename Type, typename Del = std::default_delete<Type>>
class SmartPtr : private Del {
public:
	SmartPtr(Type *ptr, Del &&del = Del()) : Del(std::move(del)), ptr(ptr) {}
	Del &del() { return *this; }
	const Del &del() const { return *this; }
	Type *get() { return ptr; }
	const Type *get() const { return ptr; }
	Type *operator*() { return ptr; }
	const Type *operator*() const { return ptr; }
	~SmartPtr() { (*this)(ptr); }
private:
	Type *ptr;
};

template <typename Type>
class Empty {
public:
	void operator()(Type *ptr) {
		std::cout << "In unallocate" << std::endl;
		ptr->~Type();
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
		SmartPtr<Sample> ptr2(new Sample("sample 2"));
		{
			SmartPtr<Sample, Empty<Sample>> ptr1(new Sample("sample 1"));
			std::cout << sizeof(ptr2) << std::endl;
		}
		std::cout << sizeof(ptr2) << std::endl;
	}

	system("pause");
}