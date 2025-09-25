#pragma once
#include "Zombie.h"
#include "ZombiePropertySheet.h"

class ZombieZcorpRacer : public Zombie
{
public:
	static void modInit();
};

class ZombieZcorpRacerProps : public ZombiePropertySheet
{
public:
	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieZcorpRacerPropsBuildSymbols;
	char pad[5];
	SexyString RacerType;

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



