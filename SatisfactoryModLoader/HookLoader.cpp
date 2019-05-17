#include "stdafx.h"
#include "HookLoader.h"

namespace SML {
	std::map<SML::Event, std::vector<void*>> HookLoader::cachedFunctions;

	void HookLoader::cache(std::map<SML::Event, std::vector<void*>> array) {
		cachedFunctions = array;
	}

	void HookLoader::hookAll(HArray array) {
		array = std::map<Event, std::vector<void*>>();

		_subscribe<Event::AFGCharacterPlayerBeginPlay>(array, "AFGCharacterPlayer::BeginPlay");
	}
}