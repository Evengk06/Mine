#pragma once

#include <util/FString.h>
#include "HookLoader.h"
#include "util/TypeNames.h"

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	namespace Objects {
		class UWorld {
		public:
			void Tick(enum ELevelTick TickType, float DeltaSeconds);
		};

		class UGameInstance {
		public:
			UWorld* GetWorld();
		};

		class UGameplayStatics {
		public:
			static class APawn* GetPlayerPawn(class UObject* WorldContextObject, int PlayerIndex);
		};

		class AFGPlayerController {
		public:
			void BeginPlay();
			void EnterChatMessage(FString *inMessage);
			void OnAttentionPingPressed();
			void Suicide();
		};

		DEFINE_METHOD(AFGPlayerController::BeginPlay, void*);

		class UFGInventoryComponent {
		public:
			void SortInventory();
		};

		class FPakPlatformFile {
		public:
			void GetPakSigningKeys(void* OutKey);
		};

	}
}

#pragma warning (pop)