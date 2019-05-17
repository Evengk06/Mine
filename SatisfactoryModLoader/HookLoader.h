#pragma once

#include <util/DetoursFwd.h>
#include <map>
#include <vector>
#include <iostream>
#include <util/Utility.h>
#include <Windows.h>

namespace SML {
	enum Event {
		AFGCharacterPlayerBeginPlay
	};

	template <typename... Args>
	using EventFunc = void(Args...);

	typedef std::map<SML::Event, std::vector<void*>>& HArray;
	static std::map<SML::Event, std::vector<void*>> modFunctions;

	class HookLoader {
	public:
		static std::map<SML::Event, std::vector<void*>> cachedFunctions;

		HookLoader() {}

		template <Event E>
		static void subscribe(HArray functions, void* function) {
			functions[E].push_back(function);
			Utility::info("Subscribed function from mod. \nEvent:", E, "\nCapacity: ", functions[E].size());
			Utility::info("MODFUNCTIONS FROM MOD: ", &functions);
		}

		static void cache(std::map<SML::Event, std::vector<void*>> array);

		static void hookAll(HArray array);
	private:
		template <Event E, typename... A>
		static void _reroute(A... args) {
			//auto obj = 0x7FFDCC840A08;
			Utility::info("MODFUNCTIONS FROM ROUTER: ", &modFunctions);
			std::vector<void*>& modEvents = cachedFunctions[E];
			Utility::info("Capacity: ", modEvents.size());
			for (size_t i = 0; i < modEvents.size(); i++) {
				auto func = *(EventFunc<A...>*)modEvents[i];
				func((args)...);
			}
		}

		template <Event E>
		static void _subscribe(HArray array, const char* hook) {
			void* o = Detours::DetourFindFunction("FactoryGame-Win64-Shipping.exe", hook);
			if (!o) {
				Utility::error("Cannot find ", hook);
				return;
			}

			Detours::DetourTransactionBegin();
			Detours::DetourUpdateThread(GetCurrentThread());
			void** v = &o;
			Detours::DetourAttach(v, &_reroute<E>);
			Detours::DetourTransactionCommit();

			modFunctions.insert(std::pair(E, std::vector<void*>()));
		}
	};

	//std::map<Event, std::vector<void*>> HookLoader::modFunctions = std::map<Event, std::vector<void*>>();
}