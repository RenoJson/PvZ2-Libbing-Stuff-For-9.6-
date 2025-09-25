#pragma once
#include "Zombie.h",
#include "Sexy/RtWeakPtr.h"
#include "../Reflection/ReflectionBuilder.h"
namespace U46Project
{
	class ZombieRedBoy : public Zombie
	{
		DECLARE_STATIC_RT_CLASS_MEMBERS();
		bool m_shiftlane;
		int m_tilereached;
		int m_tilereturned;
		int m_maxrowshift;
		std::vector<Sexy::RtWeakPtr<Projectile>> m_fireball;
		int m_fireballcount;
		float m_healthpercenttotransform;
		bool m_isyaoguaiform; 
		std::vector<Sexy::RtWeakPtr<Projectile>> m_triflame;
		int m_cdtriflame; // pvztime_t

		RT_CLASS_BUILD_SYMBOLS_BEGIN(ZombieRedBoy, Zombie);
		RT_CLASS_BUILD_SYMBOLS_END();

		DEFINE_RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZombieRedBoy, ZOMBIE_CTOR_ADDR);
		DEFINE_RT_CLASS_CONSTRUCT_FUNCTION_END();

		DEFINE_RT_CLASS_REGISTER_CLASS_FUNCTION(ZombieRedBoy);
		//DEFINE_RT_CLASS_GET_CLASS_FUNCTION(ZombieRedBoy, OFFSET_ZOMBIE_GETTYPE);

		static Sexy::RtClass* getRTClass() {
			if (s_rtClass == NULL) {
				LOGI("No global RTClass exists");
				Sexy::RtClass* rtClass = new Sexy::RtClass();
				typedef int (*rtClassRegisterClass)(void*, const char*, int, ParameterlessConstructorFunc);
				rtClassRegisterClass regrtclass = *(rtClassRegisterClass*)(*(uint*)rtClass + 0x20);
				typedef int (*rtClassGetClassFunc)();
				int parentClass = ((rtClassGetClassFunc)getActualOffset(0x89E20C))();
				regrtclass(rtClass, "ZombieRedBoy", parentClass, ZombieRedBoy::construct);
				s_rtClass = rtClass;
				LOGI("Registering class");
				ZombieRedBoy::registerClass();
			}
			LOGI("Got RTClass");
			return s_rtClass;
		};
		static void DestroyAndDealloc(ZombieRedBoy* self) {
			operator delete(self);
		}
	};
}