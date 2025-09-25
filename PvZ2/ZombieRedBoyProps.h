#pragma once
#include "Zombie.h",
#include "ZombiePropertySheet.h"
#include "Sexy/RtWeakPtr.h"
#include "../Reflection/ReflectionBuilder.h"
namespace U46Project
{
	class ZombieRedBoyProps : public ZombiePropertySheet
	{
			DECLARE_STATIC_RT_CLASS_MEMBERS();

			bool CanShiftLane = true;
			int TileBeforeReturnBack;
			int TileAfterReturnBack;
			int MaxRowShift;
			Sexy::RtWeakPtr<ProjectilePropertySheet> FireballProjectile;
			int FireballCount;
			SexyString ZombieTypeToSummonWhenTeleport;
			float HealthPercentLeftToEnableTriflame;
			Sexy::RtWeakPtr<ProjectilePropertySheet> TriflameProjectile;
			int TriflameCooldown;
			SexyString ZombieTypeToSummonWhenUseTriflame;

			RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieRedBoyProps, ZombiePropertySheet);

			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, CanShiftLane);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, TileBeforeReturnBack);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, TileAfterReturnBack);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, MaxRowShift);
			REGISTER_CLASS_RTWEAKPTR_PROPERTY(builder, rclass, ZombieRedBoyProps, FireballProjectile, ProjectilePropertySheet);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, FireballCount);
			REGISTER_STRING_PROPERTY(builder, rclass, ZombieRedBoyProps, ZombieTypeToSummonWhenTeleport);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, HealthPercentLeftToEnableTriflame);
			REGISTER_CLASS_RTWEAKPTR_PROPERTY(builder, rclass, ZombieRedBoyProps, TriflameProjectile, ProjectilePropertySheet);
			REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieRedBoyProps, TriflameCooldown);
			REGISTER_STRING_PROPERTY(builder, rclass, ZombieRedBoyProps, ZombieTypeToSummonWhenUseTriflame);

			RT_CLASS_BUILD_SYMBOLS_END();

		static void* construct() {
			auto* instance = new ZombieRedBoyProps(); 
			typedef void* (*ctorWithThisPtr)(void*); 
			ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x87AEF8); 
			baseCtor(instance); 
			setVFTable(instance, (uintptr_t)vftable); 
			return instance;
		};;

		static void registerClass() {
			typedef void* (*func123B308)(); 
			func123B308 func_0x123B308 = (func123B308)getActualOffset(0x123B308); 
			void* v0 = (void*)func_0x123B308(); 
			if (v0) {
				typedef void* (*func123B35C)(void*); 
				func123B35C func_0x123B35C = (func123B35C)getActualOffset(0x123B35C); 
				auto* builder = (Reflection::CRefManualSymbolBuilder*)func_0x123B35C(v0); 
				if (builder) {
					(builder)->RegisterClassWithProperties("ZombieRedBoyProps", ZombieRedBoyProps::buildSymbols, sizeof(ZombieRedBoyProps), 0);;
				}
			}
		};
		// this one is broken when using as getRtClass()
		// DEFINE_RT_CLASS_GET_CLASS_FUNCTION(ZombieRedBoyProps, ZOMBIEPROPS_GETTYPE);
		// this one from jay_krow repo is fine
		static Sexy::RtClass* getRTClass() { 
			if (s_rtClass == NULL) {
				Sexy::RtClass* rtClass = new Sexy::RtClass();
				typedef int (*rtClassRegisterClass)(void*, const char*, int, ParameterlessConstructorFunc);
				rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uint*)rtClass + 0x20);
				typedef int (*rtClassGetClassFunc)();
				int parentClass = ((rtClassGetClassFunc)getActualOffset(0xA01550))();
				regrtclass(rtClass, "ZombieRedBoyProps", parentClass, ZombieRedBoyProps::construct);
				s_rtClass = rtClass;
				ZombieRedBoyProps::registerClass();
			}
			return s_rtClass;
		}
		static void DestroyAndDealloc(ZombieRedBoyProps* self) {
			operator delete(self);
		}
	};
}





