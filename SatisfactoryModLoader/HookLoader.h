#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>
#include <functional>
#include "util/Utility.h"
#include "util/DetoursFwd.h"
#include "util/FunctionTraits.h"
#include "util/TypeNames.h"

/* */
struct ModReturns {
	bool useOriginalFunction = true;
};

static inline void func(){}

template <typename T, typename H>
struct Storage {
	static inline std::vector<H> hooks;
	static inline void* original;
};

template <typename... Args>
struct TupleCache {
	std::tuple<Args...> tuple;
};

template <typename... Args>
TupleCache<Args...> create_cache(Args... args) {
	return TupleCache<Args...> {
		std::tuple<Args...> {args...}
	};
}

template <auto T, typename R, auto T2, typename... Args>
class HookLoader {
public:
	typedef decltype(T) O;
	typedef decltype(T2) H;
	typedef R HandlerSignature(ModReturns*, Args...);
	typedef R ReturnType;
	typedef std::function<HandlerSignature> Handler;
	typedef R __fastcall HookType(Args...);

	typedef R HandlerSignatureNone(Args...);
	typedef std::function<HandlerSignatureNone> HandlerNone;

	static void install() {
		_install<Handler>();
		std::function<HookInfo<T, Args...>::function_type> hook = T2;
		Storage<O, Handler>::hooks.push_back(hook);
	}

	static HookType* get_original() {
		_install<HandlerNone>();
		return (HookType*)Storage<O, HandlerNone>::original;
	}

private:
	static constexpr const char _gameModule[] = "FactoryGame-Win64-Shipping.exe";

	template <typename>
	static void __fastcall apply_void(Args... args) {
		SML::Utility::warning("Hook being used");

		ModReturns returns;
		returns.useOriginalFunction = true;

		SML::Utility::warning("Hooks: ", Storage<O, Handler>::hooks.size());

		for (auto&& handler : Storage<O, Handler>::hooks)
			handler(&returns, args...);

		if (returns.useOriginalFunction)
			((HookType*)Storage<O, Handler>::original)(args...);
	}

	template <typename>
	static R __fastcall apply(Args... args) {
		ModReturns returns;
		returns.useOriginalFunction = true;

		R ret{};

		for (auto&& handler : Storage<O, Handler>::hooks)
			ret = handler(&returns, args...);

		if (returns.useOriginalFunction)
			return ((HookType*)Storage<O, Handler>::original)(args...);

		return ret;
	}

	template <typename X>
	static HookType* get_apply(std::true_type) {
		return &apply_void<X>;
	}

	template <typename X>
	static HookType* get_apply(std::false_type) {
		return &apply<X>;
	}

	template <typename F>
	static void _install() {
		const char* originalName = HookInfo<T, Args...>::name;
		SML::Utility::warning("Name: ", originalName);
		if (!Storage<O, F>::original) {
			Detours::DetourTransactionBegin();
			Storage<O, F>::original = Detours::DetourFindFunction(_gameModule, originalName);
			if (!Storage<O, F>::original) {
				SML::Utility::warning("Invalid function: ", originalName);
				return;
			}
			Detours::DetourAttach(&Storage<O, F>::original, (void*)get_apply<R>(std::is_same<R, void>{}));
			Detours::DetourTransactionCommit();
			SML::Utility::warning("Installed function: ", originalName);
		}
	}
};

template <auto Original, auto Hook, typename... Args>
void subscribe() {
	using Traits = function_traits<decltype(Original)>;
	HookLoader<Original, Traits::return_type, Hook, Args...>::install();
}

template <auto Original, typename... Args>
void call(Args... args) {
	using Traits = function_traits<decltype(Original)>;
	return HookLoader<Original, Traits::return_type, nullptr, Args...>::get_original()(args...);
}