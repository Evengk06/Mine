#pragma once
#include <../SatisfactorySDK/SDK.hpp>
#include <Windows.h>

#include <util/Objects/FFrame.h>

enum class ESpawnActorCollisionHandlingMethod : std::uint8_t {
	Undefined,
	AlwaysSpawn,
	AdjustIfPossibleButAlwaysSpawn,
	AdjustIfPossibleButDontSpawnIfColliding,
	DontSpawnIfColliding,
};

struct FActorSpawnParameters {
	FActorSpawnParameters()
		: Name()
		, Template(NULL)
		, Owner(NULL)
		, Instigator(NULL)
		, OverrideLevel(NULL)
		, SpawnCollisionHandlingOverride(ESpawnActorCollisionHandlingMethod::Undefined)
		, bRemoteOwned(0)
		, bNoFail(0)
		, bDeferConstruction(0)
		, bAllowDuringConstructionScript(0)
		, ObjectFlags(SML::Objects::EObjectFlags::RF_Transactional) {}

	/*
	A name to assign as the Name of the Actor being spawned. If no value is specified, the name of the spawned Actor will be automatically generated using the form [Class]_[Number].
	*/
	SML::Objects::FName Name;
	/*
	An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor.
	*/
	SDK::AActor* Template = nullptr;
	/*
	The Actor that spawned this Actor. (Can be left as NULL).
	*/
	SDK::AActor* Owner = nullptr;
	/*
	The APawn that is responsible for damage done by the spawned Actor. (Can be left as NULL).
	*/
	SDK::APawn* Instigator = nullptr;
	/*
	The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used.
	*/
	class	ULevel* OverrideLevel = nullptr;
	/*
	Determines whether a collision test will be performed when spawning the Actor. If true, no collision test will be performed when spawning the Actor regardless of the collision settings of the root component or template Actor.
	*/
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;
	/*
	Is the actor remotely owned.
	*/
	std::uint16_t bRemoteOwned : 1;
	/*
	Determines whether spawning will not fail if certain conditions are not met. If true, spawning will not fail because the class being spawned is `bStatic=true` or because the class of the template Actor is not the same as the class of the Actor being spawned.
	*/
	std::uint16_t	bNoFail : 1;
	/*
	Determines whether the construction script will be run. If true, the construction script will not be run on the spawned Actor. Only applicable if the Actor is being spawned from a Blueprint.
	*/
	std::uint16_t bDeferConstruction : 1;
	/*
	Determines whether or not the actor may be spawned when running a construction script. If true spawning will fail if a construction script is being run.
	*/
	std::uint16_t	bAllowDuringConstructionScript : 1;
	/*
	Flags used to describe the spawned actor/object instance.
	*/
	SML::Objects::EObjectFlags ObjectFlags;
};
