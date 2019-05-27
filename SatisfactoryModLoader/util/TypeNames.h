#pragma once

#include <functional>
#include "util/FunctionTraits.h"

struct ModReturns {
	bool use_original_function = true;
};

template <auto T, typename... Args>
struct HookInfo;

#define DEFINE_METHOD(X, ...) \
template<> struct HookInfo<&X, __VA_ARGS__> { \
static constexpr const char name[] = #X; \
using traits = function_traits<decltype(&X)>; \
typedef traits::return_type(function_type)(ModReturns*, __VA_ARGS__); \
}; \
template<> struct HookInfo<&X> { \
static constexpr const char name[] = #X; \
using traits = function_traits<decltype(&X)>; \
};