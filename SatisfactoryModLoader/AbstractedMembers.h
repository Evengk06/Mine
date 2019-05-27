#pragma once

#include "game/Global.h"
#include "Abstraction.h"

template <class Key, typename R, typename... Args>
struct Abstraction;

#define DEFINE_ROUTER(name, routed, ...) \
struct name { \
public: \
using traits = function_traits<decltype(&routed)>; \
typedef traits::return_type(function_type)(ModReturns*, __VA_ARGS__); \
static inline Abstraction<name, traits::return_type, __VA_ARGS__>* abstraction; \
};

struct Player {
	DEFINE_ROUTER(BeginPlay, SML::Objects::AFGPlayerController::BeginPlay, void*);
	DEFINE_ROUTER(Suicide, SML::Objects::AFGPlayerController::Suicide, void*);
};