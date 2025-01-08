#include <iostream>

// 模版元编程

template<long long n>
struct Factorial
{
	enum{value = n * Factorial<n - 1>::value};
};

template<>
struct Factorial<0>
{
	enum{value = 1};
};

template<long long n>
struct Factor
{
	static const long long value = n * Factor<n-1>::value; 
};

template<>
struct Factor<0>
{
	static const long long value = 1;
};

int main(const int argc, const char *argv[])
{
	(void)argc;
	(void)argv;
	std::cout << "hello world" << std::endl;
	std::cout << "Factorial<5>::value  = " << Factorial<5>::value << std::endl;
	std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
	std::cout << "Factorial<12>::value = " << Factorial<12>::value << std::endl;
	std::cout << "Factorial<15>::value = " << Factorial<15>::value << std::endl;
	std::cout << "Factorial<20>::value = " << Factorial<20>::value << std::endl;

	std::cout << "Factor<10>::value = " << Factor<10>::value << std::endl;
	std::cout << "Factor<20>::value = " << Factor<20>::value << std::endl;
}
