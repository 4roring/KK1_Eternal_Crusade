#pragma once

#include <random>

static int random_range(int _min, int _max)
{
	std::random_device _random_device;
	std::mt19937 mt(_random_device());
	std::uniform_int_distribution<int> range(_min, _max);

	return range(mt);
}