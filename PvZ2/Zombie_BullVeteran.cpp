#include "Zombie_BullVeteran.h";

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols = nullptr;

void ZombieBullVeteranProps::modInit() {
	LOGI("init bull vet props");
	FluffyHookFunction(0xA0AD5C, (void*)construct, nullptr);
	FluffyHookFunction(0xA0AEDC, (void*)ZombieBullVeteranProps::buildSymbols, (void**)&ZombieBullVeteranProps::oZombieBullVeteranPropsBuildSymbols);
	LOGI("finish init bull vet");
}