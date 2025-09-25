#include "pch.h"
#include "LevelModuleManager.h"

LevelModule* LevelModuleManager::getModule(Sexy::RtClass* rtClass) {
	typedef LevelModule* (*getModule)(LevelModuleManager*, Sexy::RtClass* rtClass);
	return ((getModule)getActualOffset(0x6FBB94))(this, rtClass);
}

void Sexy::Event::AddDelegate(DelegateBase& delegate) {
	typedef void (*AddDelegate)(Sexy::Event*, Sexy::DelegateBase*, int);
	((AddDelegate)getActualOffset(0x295854))(this, &delegate, 0);
}