#include "pch.h"
#include "RoofModuleProps.h"
#include "RoofModule.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(RoofModuleProps);

void* RoofModuleProps::GetModuleType() {
	return RoofModule::getRtClass();
}

void RoofModuleProps::modInit() {
	LOGI("RoofModuleProps mod init");

	vftable = copyVFTable(getActualOffset(0x1CF0770), 18);

	patchVFTable(vftable, (void*)RoofModuleProps::getRtClass, 0);
	patchVFTable(vftable, (void*)RoofModuleProps::GetModuleType, 14);

	RoofModuleProps::getRtClass();

	LOGI("RoofModuleProps finish init");
}