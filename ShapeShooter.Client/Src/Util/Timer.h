#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

#define TIMED_FUNCTION() Timer this_function_timer(__FUNCTION__)

class Timer
{

	const std::string func_name;

	std::chrono::steady_clock::time_point start;

public:
	Timer(const std::string& func_name) : func_name(func_name)
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		auto end = std::chrono::high_resolution_clock::now();

		
		std::chrono::duration<float, std::milli> ms = end - start;

		std::cout << func_name  << ": " << ms.count() << "ms" << std::endl;
	}
};

#endif