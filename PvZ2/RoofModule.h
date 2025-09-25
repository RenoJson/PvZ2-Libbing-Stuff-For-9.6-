#pragma once
#include "LevelModule.h"
#include "RoofModuleProps.h"

struct PamRtIdEffect {
	Sexy::RtWeakPtr<void> pamRtId;
	int offsetX;
	int offsetY;

	PamRtIdEffect(Sexy::RtWeakPtr<void>* pam, int offsetX, int offsetY) : offsetX(offsetX), offsetY(offsetY) {
		pamRtId.FromOther(pam);
	}
};

class RoofModule : public LevelModule
{
public:
	RoofModuleProps* props = NULL;

	DECLARE_STATIC_RT_CLASS_MEMBERS(RoofModule);

	DEFINE_RT_CLASS_BUILD_SYMBOLS_WITHOUT_CLASS_PROPERTIES(RoofModule, LevelModule);

	static void* construct() {
		((void)__android_log_print(ANDROID_LOG_INFO, "PVZ2ExampleMod", "Constructing ""RoofModule"));
		auto* instance = new RoofModule();
		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x53EF3C);
		baseCtor(instance);

		instance->props = NULL;

		setVFTable(instance, (uintptr_t)vftable);
		return instance;
	};

	DEFINE_RT_CLASS_GET_CLASS_FUNCTION(RoofModule, LEVELMODULE_GETCLASS_ADDR);

	DEFINE_RT_CLASS_REGISTER_CLASS_FUNCTION(RoofModule);

	RoofModuleProps* GetProps() {
		if (props == NULL) {
			props = (RoofModuleProps*)m_propertySheetPtr.Get();
		}
		return props;
	}

	static RoofModule* GetInstance();

	bool IsEffectSloped(int offsets[2], Sexy::RtWeakPtr<void>* pam);

	bool IsProjectileImmune(Projectile* proj);

	bool IsProjectileSpawnIgnoresSlope(Sexy::RtWeakPtr<ProjectilePropertySheet>* projectileProp);

	bool IsProjectileFollowsSlope(Projectile* proj);

	int GetProjectileTolerance(Projectile* proj);
};

#pragma once
