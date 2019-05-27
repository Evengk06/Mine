#pragma once

#include <vector>
#include "game/Global.h"
#include "util/TypeNames.h"
#include "util/DetoursFwd.h"
#include "util/Utility.h"
#include "AbstractedMembers.h"

template <class Key, typename R, typename... Args>
struct Abstraction {
public:
	typedef R __fastcall HookType(Args...);
	typedef R HandlerSignature(ModReturns*, Args...);
	typedef std::function<HandlerSignature> Handler;

	static inline std::vector<Handler> hooks;
	static inline const char* type;
	static inline void* original;

	template <auto RoutedFunction>
	static void createAbstraction() {
		static constexpr const char gameModule[] = "FactoryGame-Win64-Shipping.exe";
		const char* routedName = HookInfo<RoutedFunction>::name;

		void* func = Detours::DetourFindFunction(gameModule, routedName);
		if (!func) {
			SML::Utility::error("Invalid reroute: ", typeid(Key).name(), " -> \"", routedName, "\"");
			return;
		}

		Detours::DetourTransactionBegin();
		Detours::DetourAttach(&func, (void*)get_apply<R>(std::is_same<R, void>{}));
		Detours::DetourTransactionCommit();

		type = routedName;
		original = func;

		SML::Utility::warning("Rerouted: ", typeid(Key).name(), " -> \"", routedName, "\"");
		SML::Utility::warning("Args: ", typeid(Args).name());
	}

private:
	template <typename T>
	static void debug_args(typename T) {
		SML::Utility::warning("debug_args<T>: ", typeid(T).name());
	}

	template <typename T, typename... Args>
	static void debug_args(T t, Args... args) {
		SML::Utility::warning("debug_args<T, Args...>: ", typeid(T).name());
		debug_args(args...);
	}

	template <typename>
	static void __fastcall apply_void(Args... args) {
		typedef std::function<Key::function_type> std_function;

		SML::Utility::warning("Hook being used");
		debug_args(nullptr, args...);

		ModReturns returns;
		returns.use_original_function = true;

		for (auto&& handler : hooks)
			handler(&returns, args...);

		if (returns.use_original_function)
			((HookType*)original)(args...);
	}

	template <typename>
	static R __fastcall apply(Args... args) {
		typedef std::function<Key::function_type> std_function;

		ModReturns returns;
		returns.use_original_function = true;

		R ret{};

		for (auto&& handler : hooks)
			ret = handler(&returns, args...);

		if (returns.use_original_function)
			return ((HookType*)original)(args...);

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

template <class Key, auto Hook>
void subscribe() {
	typedef Key::function_type function_type;
	std::function<function_type> hook = Hook;
	auto abs = Key::abstraction;
	abs->hooks.push_back(hook);

	SML::Utility::warning("Added hook to ", typeid(Key).name());
}

void setupAbstraction() {
	Player::BeginPlay::abstraction->createAbstraction<&SML::Objects::AFGPlayerController::BeginPlay>();
}