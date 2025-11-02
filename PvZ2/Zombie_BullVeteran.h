#pragma once
#include "Zombie_Bull.h"

class ZombieBullVeteranProps : public ZombieBullProps
{
public:
	float BullRunAgainDelay = 3.0f;
	float BullRammingDamage = 1500.0f;
	SexyString VetRiderType = "west_bullrider";
	float LaunchDistance = 256.0f;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieBullVeteranPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieBullVeteranProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x87B2A4);
		baseCtor(props);

		*reinterpret_cast<int*>(uintptr_t(props)) = getActualOffset(0x1CB8D60);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieBullVeteranPropsBuildSymbols(builder, rclass);
		REGISTER_STRING_PROPERTY(builder, rclass, ZombieBullVeteranProps, VetRiderType);
		REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieBullVeteranProps, LaunchDistance);
	};

};

