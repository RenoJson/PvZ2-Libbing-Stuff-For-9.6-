#include "pch.h"
#include "PvZ2/RoofStageProps.h"
#include "PvZ2/RoofStage.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(RoofStageProps);

void* RoofStageProps::GetModuleType() {
	return RoofStage::getRtClass();
}

void RoofStageProps::modInit() {
	LOGI("RoofStageProps mod init");

	vftable = copyVFTable(getActualOffset(STAGEMODULEPROPS_VTBL), STAGEMODULEPROPS_VFUNCS);

	patchVFTable(vftable, (void*)RoofStageProps::getRtClass, 0);
	patchVFTable(vftable, (void*)RoofStageProps::GetModuleType, 14);

	RoofStageProps::getRtClass();

	LOGI("RoofStageProps finish init");
}
