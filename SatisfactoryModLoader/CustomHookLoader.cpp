#include "stdafx.h"
#include "CustomHookLoader.h"

namespace SML {
	std::map<Event, std::vector<void*>> HookLoader::modFunctions;

	void HookLoader::hookAll() {
		modFunctions = std::map<Event, std::vector<void*>>();

		_subscribe<Event::AFGCharacterPlayerBeginPlay>("AFGCharacterPlayer::BeginPlay");
	}
}