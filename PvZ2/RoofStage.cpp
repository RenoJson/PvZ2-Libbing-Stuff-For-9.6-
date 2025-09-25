#include "pch.h"
#include "PvZ2/RoofStage.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(RoofStage);

float GetSlopeOffset(float mX, float mY) {
	auto roofStage = RoofStage::Get();
	if (roofStage) {
		return roofStage->GetDepth(mX, mY);
	}
	else {
		return 0.0f;
	}
}

void RoofStage::modInit() {
	LOGI("RoofStage mod init");

	vftable = copyVFTable(getActualOffset(STAGEMODULE_VTBL), STAGEMODULE_VFUNCS);
	patchVFTable(vftable, (void*)RoofStage::getRtClass, 0);

	RoofStage::getRtClass();

	LOGI("RoofStage finish init");
}
