#pragma once

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
	};
};