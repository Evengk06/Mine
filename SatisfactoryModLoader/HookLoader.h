#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>
#include <functional>
#include "util/Utility.h"
#include "util/DetoursFwd.h"

/* */
struct ModReturns {
	bool useOriginalFunction = true;
};

template <class F>
struct function_traits;

template <class R, class...Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};

template <class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : public function_traits<R(C&, Args...)> {};

template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> : public function_traits<R(C&, Args...)> {};

template<class C, class R>
struct function_traits<R(C::*)> : public function_traits<R(C&)> {};

template <class R, class... Args>
struct function_traits<R(Args...)> {
	using return_type = R;

	static constexpr std::size_t arity = sizeof...(Args);

	template <size_t N>
	struct argument {
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		//using type = typename nth_element<N, Args...>;
	};
};

static inline void func(){}

template <typename T, typename Handler>
struct Storage {
	static inline std::vector<Handler> hooks;
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

template <typename T, typename R, typename... Args>
class HookLoader {
public:
	typedef R HandlerSignature(ModReturns*, Args...);
	typedef R ReturnType;
	typedef std::function<HandlerSignature> Handler;

	static void install(const char* originalName, Handler hook) {
		if (!Storage<T, Handler>::original) {
			Detours::DetourTransactionBegin();
			Storage<T, Handler>::original = Detours::DetourFindFunction(_gameModule, originalName);
			if (!Storage<T, Handler>::original) {
				SML::Utility::warning("Invalid function: ", originalName);
				return;
			}
			Detours::DetourAttach(&Storage<T, Handler>::original, (void*)get_apply<R>(std::is_same<R, void>{}));
			Detours::DetourTransactionCommit();
			SML::Utility::warning("Installed function: ", originalName);
		}

		Storage<T, Handler>::hooks.push_back(hook);
	}

private:
	static constexpr const char _gameModule[] = "FactoryGame-Win64-Shipping.exe";

	typedef R __fastcall HookType(Args...);

	template <typename>
	static void __fastcall apply_void(Args... args) {
		SML::Utility::warning("Hook being used");

		ModReturns returns;
		returns.useOriginalFunction = true;

		SML::Utility::warning("Hooks: ", Storage<T, Handler>::hooks.size());

		for (auto&& handler : Storage<T, Handler>::hooks)
			handler(&returns, args...);

		if (returns.useOriginalFunction)
			((HookType*)Storage<T, Handler>::original)(args...);
	}

	template <typename>
	static R __fastcall apply(Args... args) {
		ModReturns returns;
		returns.useOriginalFunction = true;

		R ret{};

		for (auto&& handler : Storage<T, Handler>::hooks)
			ret = handler(&returns, args...);

		if (returns.useOriginalFunction)
			return ((HookType*)Storage<T, Handler>::original)(args...);

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
};

template <class Original, typename Hook, typename... Args>
void subscribe(Original original, const char* originalName, Hook hook, Args... args) {
	using Traits = function_traits<Original>;

	SML::Utility::warning("Hook: ", typeid(Hook).name());
	SML::Utility::warning("Wanting: ", typeid(HookLoader<Original, Traits::return_type, Args...>::Handler).name());

	//HookLoader<Original, Traits::return_type, Args...>::install(originalName, hook);
}