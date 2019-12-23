#pragma once
#include <../SatisfactorySDK/SDK.hpp>
#include <Lib.h>

namespace SML {
	namespace Mod {
		namespace Functions {
			/**
			* This function has been largely replaced by the asset caching system. To prevent slowdowns, use that instead!
			* 
			* Load an object from a pak file. Will crash if the pak is not installed.
			*
			* The asset name must be of the following format: \\Game\\FactoryGame\\Path\\To\\Asset\\AssetFile.AssetFile
			* If you are loading a blueprint, the name must have a _C appended to the end.
			*
			* @author Brabb3l
			*/
			SML_API SDK::UObject* loadObjectFromPak(SDK::UClass *ObjectClass, const wchar_t* InName);

			/**
			* Load an object from a pak file. Will crash if the pak is not installed.
			*
			* The asset name must be of the following format: \\Game\\FactoryGame\\Path\\To\\Asset\\AssetFile.AssetFile
			* If you are loading a blueprint, the name must have a _C appended to the end.
			*
			* @author Brabb3l
			*/
			SML_API SDK::UObject* loadObjectFromPak(const wchar_t* InName);

			/**
			* Spawns an actor at a given location, when given the world.
			*
			* @author Crystallin
			*/
			SML_API SDK::UClass* spawnActor(void* UWorld, void* *UClass, void* *FVector, void* *FRotator, void* *FActorSpawnParameters);

			/**
			* Get the current global world object.
			*
			* @author SuperCoder79
			*/
			SML_API SDK::UWorld* getWorld();

			/**
			* Get the local player character
			*
			* @author SuperCoder79
			*/
			SML_API SDK::AFGCharacterPlayer* getPlayerCharacter();

			/**
			* Get the local player controller
			*
			* @author SuperCoder79
			*/
			SML_API SDK::AFGPlayerController* getPlayerController();

			/**
			* Get the game instance
			* 
			* @author SuperCoder79
			*/
			SML_API SDK::UFGGameInstance* getGameInstance();

			/**
			* Get the game state
			*
			* @author SuperCoder79
			*/
			SML_API SDK::AFGGameState* getGameState();

			/**
			* Get the game level
			* 
			* @author SuperCoder79
			*/
			SML_API SDK::ULevel* getLevel();

			/**
			* Get the game's net driver. No idea if this actually works!
			*
			* @author SuperCoder79
			*/
			SML_API SDK::UNetDriver* getNetDriver();

			/**
			* Spawn an actor at the player, offset by the XYZ values provided.
			*
			* @author Crystallin
			*/
			SML_API SDK::UClass* spawnActorAtPlayer(SDK::UObject* obj, float x, float y, float z);

			/**
			* Spawn an actor at the player.
			*
			* @author Crystallin
			*/
			SML_API SDK::UClass* spawnActorAtPlayer(SDK::UObject* obj);

			/**
			* Spawn an actor at the provided location with the provided rotation.
			*
			* @author Crystallin
			*/
			SML_API SDK::UClass* spawnActor(SDK::UObject* obj, float x, float y, float z, float pitch, float roll, float yaw);

			/**
			* Wrapper for addRecipe(SDK::UClass*) to do everything for you.
			* Call this in beginPlay().
			*
			* @author Trxnce
			*/
			SML_API void addRecipe(const wchar_t* recipeName);

			/**
			* Adds a UFGRecipe to the game.
			*
			* @author Trxnce
			*/
			SML_API void addRecipe(SDK::UClass* recipe);

			/**
			* Returns an ItemStack from an item class and an item amount.
			*
			* @author Trxnce
			*/
			SML_API SDK::FInventoryStack makeItemStack(SDK::UClass* clazz, int amount);

			/**
			* Adds an item with an amount to the player's inventory.
			*
			* @author Trxnce
			*/
			SML_API void addItemStackToPlayer(SDK::UObject* item, const int& amount);

			/**
			* Adds a stack to the player's inventory.
			*
			* @author Trxnce
			*/
			SML_API void addItemStackToPlayer(SDK::FInventoryStack stack);

			/**
			* Sends a chat message to the local player controller.
			*
			* @author Brabb3l
			*/
			SML_API void sendMessageToPlayer(std::string msg);
		}
	}
}