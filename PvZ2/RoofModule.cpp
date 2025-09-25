#include "pch.h"
#include "RoofModule.h"
#include "PvZ2/Board.h"
#include "Sexy/DelegateBase.h"
#include "RoofModuleProps.h"
#include "MessageRouter.h"
#include "Messages.h"
#include <Sexy/PopAnim.h>

DEFINE_STATIC_RT_CLASS_MEMBERS(RoofModule);

bool RoofModule::IsEffectSloped(int offsets[2], Sexy::RtWeakPtr<void>* pam) {
	for (auto& sloped : GetProps()->SlopedEffects) {
		typedef void (*getPopAnimRes)(Sexy::RtWeakPtr<Sexy::PopAnim>*, SexyString*);
		getPopAnimRes pGetPopAnimRes = (getPopAnimRes)getActualOffset(0xC9B7D4);

		Sexy::RtWeakPtr<Sexy::PopAnim> popAnimPtr;
		pGetPopAnimRes(&popAnimPtr, &sloped.Name);

		auto popAnim = popAnimPtr.Get();

		Sexy::RtWeakPtr<void> slopedPam;
		slopedPam.FromOther(&popAnim->pamRtId);
		if (slopedPam.Equals(pam)) {
			// LOGI("Effect match detected");
			offsets[0] = sloped.OffsetX;
			offsets[1] = sloped.OffsetY;
			return true;
		}
	}

	return false;
}

inline bool IsProjectileInList(Projectile* proj, const std::vector<Sexy::RtWeakPtr<ProjectilePropertySheet>>& list) {
	for (auto& projectileProp : list) {
		if (proj->m_propertySheetPtr.Equals((Sexy::RtWeakPtr<ProjectilePropertySheet>*) & projectileProp)) {
			return true;
		}
	}

	return false;
}

bool RoofModule::IsProjectileImmune(Projectile* proj) {
	auto props = GetProps();
	return IsProjectileInList(proj, props->ImmuneToSlope);
}

/*
bool RoofModule::IsProjectileSpawnIgnoresSlope(Projectile* proj) {
	auto props = GetProps();
	return IsProjectileInList(proj, props->SpawnIgnoresSlope);
}
*/

bool RoofModule::IsProjectileSpawnIgnoresSlope(Sexy::RtWeakPtr<ProjectilePropertySheet>* projectileProp) {
	auto props = GetProps();
	for (auto& prop : props->SpawnIgnoresSlope) {
		if (projectileProp->Equals(&prop)) {
			return true;
		}
	}
	return false;
}

bool RoofModule::IsProjectileFollowsSlope(Projectile* proj) {
	auto props = GetProps();
	return IsProjectileInList(proj, props->FollowsSlope);
}

int RoofModule::GetProjectileTolerance(Projectile* proj) {
	auto props = GetProps();
	for (auto& killTolerance : props->KillTolerances) {
		if (proj->m_propertySheetPtr.Equals((Sexy::RtWeakPtr <ProjectilePropertySheet> *) & killTolerance.Projectile)) {
			return killTolerance.ExtraHeight;
		}
	}
	return 0;
}

RoofModule* RoofModule::GetInstance() {
	return (RoofModule*)getBoard()->m_levelModuleManager->getModule(RoofModule::getRtClass());
}

void RoofModule::modInit() {
	LOGI("RoofModule init");

	vftable = copyVFTable(getActualOffset(LEVELMODULE_VTBL), LEVELMODULE_VTBL_NFUNC);

	patchVFTable(vftable, (void*)RoofModule::getRtClass, 0);

	getRtClass();

	LOGI("RoofModule finish init");
}
