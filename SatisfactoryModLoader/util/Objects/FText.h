#pragma once

#include "FString.h"

namespace SML {
	namespace Objects {
		struct __declspec(align(8)) FText {
			void* TextData;
			unsigned int Flags;

			static FText fromString(FString&& str);
		};

	}
}