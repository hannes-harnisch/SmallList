#define HH_ASSERT(condition, message)                                                                                          \
	{                                                                                                                          \
		if(!(condition))                                                                                                       \
		{                                                                                                                      \
			__debugbreak();                                                                                                    \
			std::abort();                                                                                                      \
		}                                                                                                                      \
	}

#include "../Include/SmallList.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace hh;
using namespace std;
