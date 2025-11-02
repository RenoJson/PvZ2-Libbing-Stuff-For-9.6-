#pragma once
#include "Zombie.h"
#include "ZombiePropertySheet.h"


class ZombieZcorpRacerProps : public ZombiePropertySheet
{
public:
	Rect RacerRect = { 10, 10, 40, 80 };
	float ChairCollideDamage = 5000.0f;
	float RacerSpeed = 1.0f;
	float LaunchZombieDistance = 2.5f;
	float LaunchZombieApex = 75.0f;
	float LaunchZombieTimeToLand = 2.5f;
	SexyString RacerType = "zcorp_racer";

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieZcorpRacerPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieZcorpRacerProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x87B2A4);
		baseCtor(props);

		*reinterpret_cast<int*>(uintptr_t(props)) = getActualOffset(0x1CA9FD8);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieZcorpRacerPropsBuildSymbols(builder, rclass);
		REGISTER_STRING_PROPERTY(builder, rclass, ZombieZcorpRacerProps, RacerType);
	};
};



