#pragma once

#include <algorithm>
#include <iostream>
#include <mutex>
#include <string>

extern std::mutex locLogMutex;

int
Random(
	const int	aFrom,
	const int	aTo);

class Configuration //centralized controller
{
public:
	unsigned int buildFloor;
	unsigned int humansCount;
	unsigned int elevatorsCount;
};

template <typename T>
void
PrivLog(
	T aT)
{
	std::cout << aT << std::endl;
}

template<typename T, typename... Args>
void
PrivLog(
	T aT,
	Args... aArgs)
{
	std::cout << aT << " ";

	PrivLog(aArgs...);
}

template<typename... Args>
void
Log(
	Args... aArgs)
{
	std::unique_lock<std::mutex> lock(locLogMutex);

	PrivLog(aArgs...);
}