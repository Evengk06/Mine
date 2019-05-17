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

	class HookLoader {
	public:
		static std::map<Event, std::vector<void*>> modFunctions;

		HookLoader() {}

		template <Event E>
		static void subscribe(void* function) {
			modFunctions[E].push_back(function);
			Utility::info("Subscribed function from mod. \nEvent:", E, "\nCapacity: ", modFunctions[E].size());
		}

		static void hookAll();
	private:
		template <Event E, typename... A>
		static void _reroute(A... args) {
			std::vector<void*> modEvents = modFunctions[E];
			Utility::info("Capacity: ", modEvents.size());
			for (size_t i = 0; i < modEvents.size(); i++) {
				auto func = *(EventFunc<A...>*)modEvents[i];
				func((args)...);
			}
		}

		template <Event E>
		static void _subscribe(const char* hook) {
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