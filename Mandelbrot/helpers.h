#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

inline void out(const u64 o) 
{
	std::cout << "Result: " << o << std::endl;
}

inline u32 get_option(std::string q, std::vector<std::string> a)
{
	u32 option = 0;
	do {
		std::cout << q << std::endl;
		for (size_t i = 0; i < a.size(); ++i)
		{
			std::cout << i+1 << ": " << a[i] << std::endl;
		}
		std::cout << "Option: " << std::flush;
		std::string in;
		std::cin >> in;
		std::stringstream ss;
		ss << in;
		ss >> option;
		if (option && option <= a.size())
		{
			return option;
		}
		std::cout << "Please enter a number from 1 to " << a.size() << std::endl;
	} while (true);
}

inline u64 get_number(std::string q)
{
	u64 answer = 0;
	do
	{
		std::cout << q << std::endl;
		std::cout << "?: " << std::flush;
		std::string in;
		std::cin >> in;
		std::stringstream ss;
		ss << in;
		ss >> answer;
		if (answer) return answer;
		std::cout << "Please enter a number! (non zero)" << std::endl;
	} while (true);
}

inline double get_double(std::string q)
{
	auto answer = 0.;
	do
	{
		std::cout << q << std::endl;
		std::cout << "?: " << std::flush;
		std::string in;
		std::cin >> in;
		std::stringstream ss;
		ss << in;
		ss >> answer;
		if (answer != 0.) return answer;
		std::cout << "Please enter a number! (non zero)" << std::endl;
	} while (true);
}

inline std::string get_string(std::string q)
{
	std::cout << q << std::endl;
	std::cout << "?: " << std::flush;
	std::string in;
	std::cin >> in;
	return in;
}
