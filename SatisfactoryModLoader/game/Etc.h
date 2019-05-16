#pragma once

#include <util/FString.h>

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	namespace Objects {
		class UFGItemDescriptor {
		public:
			static void Constructor(UFGItemDescriptor *thiz);
		};

		template <typename TElement, typename TAllocator>
		struct TArray;

		class AActor {
		public:
			template <typename TComponent, typename TAllocator>
			void GetComponents(TArray<TComponent*, TAllocator>* OutComponents, bool bIncludeFromChildActors);
		};

		class UFGInventoryComponent;
		class FDefaultAllocator;
	}
}

#pragma warning (pop)