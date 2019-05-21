#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <cassert>
#include <iostream>
#include <functional>
#include <util/Utility.h>
#include <util/DetoursFwd.h>

///LOG(typeid(decltype(ExampleMod::beginPlay(std::declval<ModReturns*>(), std::declval<void*>()))).name());
//template <class T>
//void get_return_type() {
//	decltype(T())
//}

//template <size_t I, class... Types>
//constexpr std::tuple_element_t <I, std::tuple<Types...>>& get(std::tuple<Types...>&) noexcept;
//
//template <size_t I, class... Types>
//constexpr const std::tuple_element_t <I, std::tuple<Types...>>& get(const std::tuple<Types...>&) noexcept;
//
//template<
//	typename Tuple,
//	typename Indices = std::make_index_sequence<std::tuple_size<Tuple>::value>>
//	struct runtime_get_func_table;
//
//template<typename Tuple, size_t ... Indices>
//struct runtime_get_func_table<Tuple, std::index_sequence<Indices...>> {
//	using return_type = typename std::tuple_element<0, Tuple>::type&;
//	using get_func_ptr = return_type(*)(Tuple&) noexcept;
//	static constexpr get_func_ptr table[std::tuple_size<Tuple>::value] = {
//		&std::get<Indices>...
//	};
//};
//
//template<typename Tuple, size_t ... Indices>
//constexpr typename
//runtime_get_func_table<Tuple, std::index_sequence<Indices...>>::get_func_ptr
//runtime_get_func_table<Tuple, std::index_sequence<Indices...>>::table[
//	std::tuple_size<Tuple>::value];
//
//template<typename Tuple>
//constexpr
//typename std::tuple_element<0, typename std::remove_reference<Tuple>::type>::type&
//runtime_get(Tuple&& t, size_t index) {
//	using tuple_type = typename std::remove_reference<Tuple>::type;
//	if (index >= std::tuple_size<tuple_type>::value)
//		throw std::runtime_error("Out of range");
//	return runtime_get_func_table<tuple_type>::table[index](t);
//}
//
//
//template <size_t I>
//struct visit_impl
//{
//	template <typename T, typename F>
//	static void visit(T& tup, size_t idx, F fun)
//	{
//		if (idx == I - 1) fun(std::get<I - 1>(tup));
//		else visit_impl<I - 1>::visit(tup, idx, fun);
//	}
//};
//
//template <>
//struct visit_impl<0>
//{
//	template <typename T, typename F>
//	static void visit(T& tup, size_t idx, F fun) { assert(false); }
//};
//
//template <typename F, typename... Ts>
//void visit_at(std::tuple<Ts...> const& tup, size_t idx, F fun)
//{
//	visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
//}
//
//template <typename F, typename... Ts>
//void visit_at(std::tuple<Ts...>& tup, size_t idx, F fun)
//{
//	visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
//}




//template <std::size_t n, typename = std::make_index_sequence<n>>
//struct nth_element_impl;
//
//template <std::size_t n, std::size_t ...ignore>
//struct nth_element_impl<n, std::index_sequence<ignore...>> {
//	template <typename Tn>
//	static Tn f(decltype((void*)ignore)..., Tn*, ...);
//};
//
//template <typename T>
//struct wrapper { using type = T; };
//
//template <std::size_t n, typename ...T>
//using nth_element = typename decltype(
//	nth_element_impl<n>::f(static_cast<wrapper<T>*>(0)...)
//	)::type;

/* */
struct ModReturns {
	bool useOriginalFunction = true;
};

/* */
//template <typename... Args>
//struct Types {
//	void create_tuple(Args... args) {
//		auto t = std::tuple<Args...>{ args... };
//	}
//};
//
//
//template <std::size_t I, typename T, typename ...Ts>
//struct nth_element_impl {
//	using type = typename nth_element_impl<I - 1, Ts...>::type;
//};
//
//template <typename T, typename ...Ts>
//struct nth_element_impl<0, T, Ts...> {
//	using type = T;
//};
//
//template <std::size_t I, typename ...Ts>
//using nth_element = typename nth_element_impl<I, Ts...>::type;
//




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

	//template <size_t N>
	//struct argument {
	//	//using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	//	using type = typename nth_element<N, Args...>;
	//};
};

static void func(){}

template <typename T, typename Handler>
struct Storage {
	static std::vector<Handler> hooks;
	static void* original;
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
struct HookLoader {
public:
	typedef R HandlerSignature(ModReturns*, Args...);
	typedef R ReturnType;
	typedef std::function<HandlerSignature> Handler;

	static void install() {
		install_hook();
	}

private:
	static constexpr const char _gameModule[] = "FactoryGame-Win64-Shipping.exe";

	typedef R __fastcall HookType(Args...);

	template <typename>
	static void __fastcall apply_void(Args... args) {
		ModReturns returns;
		returns.useOriginalFunction = true;

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

	static void install_hook() {
		Detours::DetourTransactionCommit();
		//if (!Storage<T, Handler>::original) {
			//Detours::DetourTransactionBegin();
			/*Storage<T, Handler>::original = Detours::DetourFindFunction(_gameModule, typeid(T).name());
			Detours::DetourAttach(&Storage<T, Handler>::original, (void*)get_apply<R>(std::is_same<R, void>{}));
			Detours::DetourTransactionCommit();*/
			//SML::Utility::warning("Installed hook: ", typeid(T).name());
		//}
	}
};

template <class Original, typename Hook, typename... Args>
void subscribe(Original original, Hook hook, Args... args) {
	using Traits = function_traits<Original>;
	HookLoader<Original, Traits::return_type, Args...>::install();
}

//template <class F, class H>
//void subscribe(F f, typename HookLoader<H, H>::Handler handler) {
//	using Traits = function_traits<decltype(f)>;
//	HookLoader<F, Traits::return_type>::install();
//
//	SML::Utility::warning("Return Type: ", typeid(Traits::return_type).name());
//
//	size_t argSize = Traits::arity;
//	SML::Utility::warning("Arg Count: ", argSize);
//
//	//Types<T> types;
//
//	//size_t num = 0;
//	//Traits::get_arg(num);
//	//const char* name = typeid(Traits::argument<num>::type).name();
//	//std::cout << name << std::endl;
//	/*const char* name2 = typeid(Traits::argument<1>::type).name();
//	std::cout << name2 << std::endl;*/
//	/*const char* name3 = typeid(Traits::argument<2>::type).name();
//	std::cout << name3 << std::endl;*/
//	//SML::Utility::warning("Arg [0] ", typeid(Traits::argument<0>::type).name());
//	//for (size_t i = 0; i < argSize; i++) {
//	//	SML::Utility::warning("Arg [", i, "] ", typeid(Traits::argument<0>).name());
//	//	//SML::Utility::warning("Arg [", i, "] ", typeid(runtime_get(Traits::args, i)).name());
//	//	//auto t = runtime_get(Traits::args, i);
//	//}
//}

/* */
//struct Storage {
//	static std::vector<void*> hooks;
//	static void* original;
//};
//
///* */
//template <typename T>
//struct HookName {
//	const char* name;
//
//	HookName() {
//		name = typeid(T).name();
//	}
//};
//
///* */
//template <typename R, typename T, typename... A>
//struct HookSignature {
//	typedef R Signature(ModReturns*, A...);
//	std::function<Signature> function;
//
//	static HookSignature<R, T, A...> create(std::function<Signature> func) {
//		return HookSignature<R, T, A...> {
//			func
//		};
//	}
//};
//

//static void test() {
//
//}

/* */
//template <typename O, typename H>
//void subscribe(O o, H h) {
//	//auto t = fu::unwrap_function<test>;
//	//SML::Utility::info("Type: ", t);
//
//	//HookLoader<R, O>::subscribe();
//}

//template <auto O>
//void subscribe(typename HookLoader<decltype(O), O>::Handler handler) {
//	//HookLoader<decltype(O), O>::subscribe(handler);
//}

//#if defined(HOOKLOADER_EXPORTS)
//#if defined(SATISFACTORYMODLOADER_EXPORTS)
//#define SML_HOOK_API __declspec(dllexport)
//#else
//#error Exports must only be done from ModLoader dll
//#endif

// Make method available for hooking in mod launcher dll.
// Usage: EXPORT_METHOD(&Foo::Bar).
//#define EXPORT_METHOD(...) \
//template struct SML_HOOK_API HookLoader<decltype(&__VA_ARGS__), &__VA_ARGS__>; \
//template SML_HOOK_API void subscribe<&__VA_ARGS__>(HookLoader<decltype(&__VA_ARGS__), &__VA_ARGS__>::Handler handler);
//#else
//#if defined(SATISFACTORYMODLOADER_EXPORTS)
//#define SML_HOOK_API
//#else
//#define SML_HOOK_API __declspec(dllimport)
//#endif
//
//// Import method from mod launcher dll.
//// Usage: EXPORT_METHOD(&Foo::Bar).
//#define EXPORT_METHOD(...) \
//extern template struct SML_HOOK_API HookLoader<decltype(&__VA_ARGS__), &__VA_ARGS__>; \
//extern template SML_HOOK_API void subscribe<&__VA_ARGS__>(HookLoader<decltype(&__VA_ARGS__), &__VA_ARGS__>::Handler handler);
//#endif
//
//// Make method available for hooking in mod API.
//// Usage: DEFINE_METHOD(&Foo::Bar).
//#define DEFINE_METHOD(PMF) \
//template <> struct HookName<&PMF> { static constexpr const char Name[] = #PMF; }; \
//EXPORT_METHOD(PMF)
//
//// Make method available as constructor for hooking in mod API.
//// Usage: DEFINE_CONSTRUCTOR(Foo, Constructor).
//#define DEFINE_CONSTRUCTOR(CLASS, STATICMETHOD) \
//template <> struct HookName<&CLASS::STATICMETHOD> { static constexpr const char Name[] = #CLASS "::" #CLASS; }; \
//EXPORT_METHOD(CLASS::STATICMETHOD)
//
//// Make method available for hooking in mod API.
//// Usage: DEFINE_METHOD_PRE("U", &Foo::Bar) would search for "UFoo::Bar".
//#define DEFINE_METHOD_PRE(PREFIX, PMF) \
//template <> struct HookName<&PMF> { static constexpr const char Name[] = PREFIX #PMF; }; \
//EXPORT_METHOD(PMF)