#include <stdafx.h>

#include "UProperty.h"
#include "FMemory.h"

namespace SML {
	namespace Objects {
		UClass* UProperty::staticClass() {
			static auto ptr = UObject::findClass("Class CoreUObject.Field");
			return ptr;
		}

		void UProperty::initValue(void* ptr) {
			std::memset(ptr, 0, this->elemSize);
			((void(**)(UProperty*, void*)) this->vtable)[0x5D](this, ptr);
		}

		bool UBoolProperty::isSet(void* container) {
			return *getValue<char>(container) & bMask;
		}
	}
}