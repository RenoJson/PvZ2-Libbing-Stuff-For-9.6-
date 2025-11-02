#pragma once
#include "Zombie.h"
#include "ZombiePropertySheet.h"

// about HIDING_LAYER_AFTER_THROW_FUNC 0x756A98, this function is called when the bull throw the rider successful to hide the hardcode layer
// SUB_736DE4 is use for launch action frame to get the rider animrig

#define UNKFUNC_SUB_736DE4 0x736DE4 
#define UNKFUNC_GET_BOARD 0x949EFC 

class ZombieBullProps : public ZombiePropertySheet
{
public:
	Rect BuckRect = { -95, 10, 40, 125 };
	float BullEnterSpeed = 0.30f;
	float BullPawLocation = 810.0f;
	float BullRunSpeed = 1.0f;
	SexyString RiderType = "west_bullrider";
	std::vector<SexyString> RiderLayerToHide;
	float LaunchDistance = 256.0f;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieBullPropsBuildSymbols;
	static void modInit();

	static void* construct()
	{
		auto* props = new ZombieBullProps();

		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x87B2A4);
		baseCtor(props);

		*reinterpret_cast<int*>(uintptr_t(props)) = getActualOffset(0x1CB8D14);
		return props;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieBullPropsBuildSymbols(builder, rclass);
		REGISTER_STRING_PROPERTY(builder, rclass, ZombieBullProps, RiderType);
		REGISTER_STRING_VECTOR_PROPERTY(builder, rclass, ZombieBullProps, RiderLayerToHide);
		REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieBullProps, LaunchDistance);
	};
};




