#pragma once
#include "LevelModuleProperties.h"
#include "Reflection/RCustomType/TStdVectorManipulator.h"
#include "Projectile.h"

struct SlopedEffect {
	SexyString Name;
	int OffsetX;
	int OffsetY;

	RT_CLASS_BUILD_SYMBOLS() {
		REGISTER_STRING_PROPERTY(builder, rclass, SlopedEffect, Name);
		REGISTER_INT_PROPERTY(builder, rclass, SlopedEffect, OffsetX);
		REGISTER_INT_PROPERTY(builder, rclass, SlopedEffect, OffsetY);
	}
};

struct KillTolerance {
	Sexy::RtWeakPtr<ProjectilePropertySheet> Projectile;
	int ExtraHeight;

	RT_CLASS_BUILD_SYMBOLS() {
		REGISTER_CLASS_RTWEAKPTR_PROPERTY(builder, rclass, KillTolerance, Projectile, ProjectilePropertySheet);
		REGISTER_STANDARD_PROPERTY(builder, rclass, KillTolerance, ExtraHeight);
	}
};

class RoofModuleProps : public LevelModuleProperties
{
public:
	std::vector<SlopedEffect> SlopedEffects;
	std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>> SpawnIgnoresSlope;
	std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>> FollowsSlope;
	std::vector<KillTolerance> KillTolerances;
	std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>> ImmuneToSlope;

	DECLARE_STATIC_RT_CLASS_MEMBERS(RoofModuleProps);

	RT_CLASS_BUILD_SYMBOLS_BEGIN(RoofModuleProps, LevelModuleProperties)
		REGISTER_CLASS_VECTOR_PROPERTY(builder, rclass, RoofModuleProps, SlopedEffects, SlopedEffect);
	REGISTER_RTWEAKPTR_VECTOR_PROPERTY(builder, rclass, RoofModuleProps, SpawnIgnoresSlope, ProjectilePropertySheet);
	REGISTER_RTWEAKPTR_VECTOR_PROPERTY(builder, rclass, RoofModuleProps, FollowsSlope, ProjectilePropertySheet);
	REGISTER_RTWEAKPTR_VECTOR_PROPERTY(builder, rclass, RoofModuleProps, ImmuneToSlope, ProjectilePropertySheet);
	REGISTER_CLASS_VECTOR_PROPERTY(builder, rclass, RoofModuleProps, KillTolerances, KillTolerance);
	RT_CLASS_BUILD_SYMBOLS_END();

	static void* construct() {
		((void)__android_log_print(ANDROID_LOG_INFO, "PVZ2ExampleMod", "Constructing ""RoofModuleProps"));
		auto* instance = new RoofModuleProps();
		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x53EF3C);

		instance->SlopedEffects.clear();
		instance->SlopedEffects.shrink_to_fit();

		instance->SpawnIgnoresSlope.clear();
		instance->SpawnIgnoresSlope.shrink_to_fit();

		instance->FollowsSlope.clear();
		instance->FollowsSlope.shrink_to_fit();

		instance->KillTolerances.clear();
		instance->KillTolerances.shrink_to_fit();

		instance->ImmuneToSlope.clear();
		instance->ImmuneToSlope.shrink_to_fit();

		baseCtor(instance);
		setVFTable(instance, (uintptr_t)vftable);
		return instance;
	};

	DEFINE_RT_CLASS_GET_CLASS_FUNCTION(RoofModuleProps, LEVELMODULEPROPS_GETCLASS_ADDR);

	static void registerClass() {
		typedef void* (*func123B308)();
		func123B308 func_0x123B308 = (func123B308)getActualOffset(0x123B308);
		void* v0 = (void*)func_0x123B308();
		if (v0) {
			typedef void* (*func123B35C)(void*);
			func123B35C func_0x123B35C = (func123B35C)getActualOffset(0x123B35C);
			auto* builder = (Reflection::CRefManualSymbolBuilder*)func_0x123B35C(v0);
			if (builder) {
				(builder)->RegisterClassWithProperties("SlopedEffect", SlopedEffect::buildSymbols, sizeof(SlopedEffect), 0);
				(builder)->RegisterClassWithProperties("KillTolerance", KillTolerance::buildSymbols, sizeof(KillTolerance), 0);
				(builder)->RegisterClassWithProperties("RoofModuleProps", RoofModuleProps::buildSymbols, sizeof(RoofModuleProps), 0);;
			}
		}
	};

	static void* GetModuleType();
};

