#pragma once

#include <util/FString.h>

#pragma warning (push)
#pragma warning (disable: 4667)

namespace SML {
	namespace Objects {
		class AFGCharacterBase {
		public:
			void BeginPlay();
			void GetHealthComponent(struct FFrame*, void* const);
		};

		class AFGCharacterPlayer : public AFGCharacterBase
		{
		public:
			void BeginPlay();
			void HotKeyDismantle();
			void OnUsePressed();
			void SprintPressed();
		};

		class HealthComponent {
		public:
			void TakeDamage(class AActor* damagedActor, float damageAmount, class UDamageType *damageType, class AController *instigatedBy, class AActor *damageCauser);
		};

		class UFGBuildGunState {
		public:
			float GetBuildGunDelayPercentage();
		};

		class AFGBuildGun {
		public:
			void  OnNoSnapModePressed();
			void  OnPrimaryFirePressed();
			void  OnScrollDownPressed();
			void  OnScrollModePressed();
			void  OnSnapToGuideLinesPressed();
		};

		class AFGC4Dispenser {
		public:
			void  OnDetonatePressed();
			void  OnPrimaryFirePressed();
		};

		class AFGChainsaw {
		public:
			void  OnPrimaryFirePressed();
		};

		class AFGColorGun {
		public:
			void  OnSecondaryFirePressed();
		};


		class AFGConsumableEquipment {
		public:
			void  OnPrimaryFirePressed();
		};


		class AFGEquipmentStunSpear {
		public:
			void  OnFirePressed();
		};

		class AFGHookshot {
		public:
			void  OnPrimaryFirePressed();
			void  OnSecondaryFirePressed();
		};

		class AFGPortableMinerDispenser {
		public:
			void  OnPrimaryFirePressed();
		};

		class AFGResourceScanner {
		public:
			void  OnScanPressed();
		};

		class AFGWeapon {
		public:
			void  OnPrimaryFirePressed();
			void  OnReloadPressed();
		};

		class UFGBuildGunStateDismantle {
		public:
			void  BeginBuildGunDelay();
			void  ResetBuildGunDelay();
		};

	}
}

#pragma warning (pop)