#pragma once
template <auto T>
struct HookName;

#define DEFINE_METHOD(X) \
template<> struct HookName<&X> { static constexpr const char name[] = #X; };