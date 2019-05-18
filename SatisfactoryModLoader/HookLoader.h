#pragma once

#include <util/DetoursFwd.h>
#include <map>
#include <vector>
#include <iostream>
#include <util/Utility.h>
#include <Windows.h>

namespace SML {
	enum RunEvent {
		Ignore,
		Run,
		DontRun
	};

	enum Event {
		AFGCharacterPlayerBeginPlay,
		AFGPlayerControllerBeginPlay
	};

	struct HookedFunction {
		void* original;
		std::vector<void*> hooks;
	};

	template <typename T>
	struct HookReturn {
	public:
		T* value;
		RunEvent run;
	};

	template <typename T, typename... Args>
	using EventFunc = void(HookReturn<T>*, Args...);

	typedef std::map<SML::Event, HookedFunction>& HArray;
	static std::map<SML::Event, HookedFunction> modFunctions;

	class HookLoader {
	public:
		static std::map<SML::Event, HookedFunction> cachedFunctions;

		HookLoader() {}

		template <Event E>
		static void subscribe(HArray functions, void* function) {
			functions[E].hooks.push_back(function);
		}

		static void cache(std::map<SML::Event, HookedFunction> array);

		static void hookAll(HArray array);
	private:
		template <Event E, typename... A>
		static void _rerouteVoid(A... args) {
			HookedFunction hooked = cachedFunctions[E];
			Utility::info("IN HOOK: ", hooked.original);
			std::vector<void*>& modEvents = hooked.hooks;
			std::vector<HookReturn<void>> results = {};

			for (size_t i = 0; i < modEvents.size(); i++) {
				auto func = *(EventFunc<void, A...>*)modEvents[i];
				HookReturn<void> obj;
				func(&obj, (args)...);
				results.push_back(obj);
			}

			std::vector<std::tuple<HookReturn<void>, HookReturn<void>>> conflicts = {};

			int ignore = 0;
			int run = 0;
			int dontRun = 0;
			for (size_t i = 0; i < results.size() - 1; i++) {
				for (size_t j = 1; j < results.size(); j++) {
					HookReturn<void> a = results[i];
					HookReturn<void> b = results[j];

					if (a.value == NULL || b.value == NULL || a.value == b.value) {
						ignore += a.run == RunEvent::Ignore ? 1 : 0;
						run += a.run == RunEvent::Run ? 1 : 0;
						dontRun += a.run == RunEvent::DontRun ? 1 : 0;
						continue;
					}

					conflicts.push_back(std::make_tuple(a, b));
				}
			}

			HookReturn<void> lastB = results[results.size() - 1];
			ignore += lastB.run == RunEvent::Ignore ? 1 : 0;
			run += lastB.run == RunEvent::Run ? 1 : 0;
			dontRun += lastB.run == RunEvent::DontRun ? 1 : 0;

			if (conflicts.size() == 0) {
				if (ignore >= 0 && run >= 0 && dontRun == 0) {
					Utility::info("[RUN]");
					// run
					auto func = ((void(*)(A...))hooked.original);
					func((args)...);
				}
				else if (ignore >= 0 && run == 0 && dontRun >= 0) {
					Utility::info("[DONT RUN]");
					// dont run
				}
				else {
					for (auto pair : conflicts) {
						Utility::error("Calling the ", E, " hook resulted in a conflict. Result[0]=", std::get<0>(pair).value, " Result[1]=", std::get<1>(pair).value);
					}
					// conflict
					return;
				}

				return;
			}

			for (auto pair : conflicts) {
				Utility::error("Calling the ", E, " hook resulted in a conflict. Result[0]=", std::get<0>(pair).value, " Result[1]=", std::get<1>(pair).value);
			}

			return;
		}

		template <Event E, typename T, typename... A>
		static T _reroute(A... args) {
			HookedFunction& hooked = cachedFunctions[E];
			std::vector<void*>& modEvents = hooked.hooks;
			std::vector<HookReturn<T>> results = {};

			 T currentResult = NULL;

			for (size_t i = 0; i < modEvents.size(); i++) {
				auto func = *(EventFunc<T, A...>*)modEvents[i];
				HookReturn<T> obj;
				func(&obj, (args)...);
				results.push_back(obj);
				if (obj.value != NULL) {
					currentResult = *obj.value;
				}
			}

			std::vector<std::tuple<HookReturn<T>, HookReturn<T>>> conflicts = {};

			int ignore = 0;
			int run = 0;
			int dontRun = 0;
			for (size_t i = 0; i < results.size() - 1; i++) {
				for (size_t j = 1; j < results.size(); j++) {
					HookReturn<T> a = results[i];
					HookReturn<T> b = results[j];

					if (a.value == NULL || b.value == NULL || a.value == b.value) {
						ignore += a.run == RunEvent::Ignore ? 1 : 0;
						run += a.run == RunEvent::Run ? 1 : 0;
						dontRun += a.run == RunEvent::DontRun ? 1 : 0;
						continue;
					}

					conflicts.push_back(std::make_tuple(a, b));
				}
			}

			HookReturn<T> lastB = results[results.size() - 1];
			ignore += lastB.run == RunEvent::Ignore ? 1 : 0;
			run += lastB.run == RunEvent::Run ? 1 : 0;
			dontRun += lastB.run == RunEvent::DontRun ? 1 : 0;

			if (conflicts.size() == 0) {
				if (ignore >= 0 && run >= 0 && dontRun == 0) {
					Utility::info("[RUN]");
					// run
					typedef T(*fptr)(A...);
					auto func = *(fptr)hooked.original;
					
					/// ERRORS
					auto ret = func((args)...);
					return ret;
				}
				else if (ignore >= 0 && run == 0 && dontRun >= 0) {
					Utility::info("[DONT RUN]");
					// dont run
				}
				else {
					for (auto pair : conflicts) {
						Utility::error("Calling the ", E, " hook resulted in a conflict. Result[0]=", std::get<0>(pair).value, " Result[1]=", std::get<1>(pair).value);
					}
					// conflict
					return NULL;
				}

				return currentResult;
			}

			for (auto pair : conflicts) {
				Utility::error("Calling the ", E, " hook resulted in a conflict. Result[0]=", std::get<0>(pair).value, " Result[1]=", std::get<1>(pair).value);
			}

			return NULL;
		}

		static void* _beginHook(const char* hook) {
			void* o = Detours::DetourFindFunction("FactoryGame-Win64-Shipping.exe", hook);
			if (!o) {
				return NULL;
			}
			return o;
		}

		template <Event E>
		static void _endHook(HArray array, void* o, void* v2, const char* hook) {
			void** v = &o;
			Detours::DetourAttach(v, v2);
			Detours::DetourTransactionCommit();

			HookedFunction oHook{
				o, std::vector<void*>()
			};
			array.insert(std::pair(E, oHook));

			Utility::info("Hooked: ", hook);
			Utility::info("IN HOOKKK: ", array[Event::AFGCharacterPlayerBeginPlay].original);
		}

		template <Event E>
		static void _subscribe(HArray array, const char* hook) {
			void* o = _beginHook(hook);
			if (o == NULL) {
				Utility::error("Cannot find ", hook);
				return;
			}
			Detours::DetourTransactionBegin();
			Detours::DetourUpdateThread(GetCurrentThread());
			void* v2 = _rerouteVoid<E>;
			_endHook<E>(array, o, v2, hook);
		}

		template <Event E, typename T>
		static void _subscribe(HArray array, const char* hook) {
			void* o = _beginHook(array, hook);
			if (o == NULL) {
				Utility::error("Cannot find ", hook);
				return;
			}
			Detours::DetourTransactionBegin();
			Detours::DetourUpdateThread(GetCurrentThread());
			void* v2 = _reroute<E, T>;
			_endHook<E>(array, o, v2, hook);
		}
	};

	//std::map<Event, std::vector<void*>> HookLoader::modFunctions = std::map<Event, std::vector<void*>>();
}