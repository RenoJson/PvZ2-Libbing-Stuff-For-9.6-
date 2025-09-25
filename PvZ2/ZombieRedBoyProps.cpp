#include "ZombieRedBoyProps.h"

void* U46Project::ZombieRedBoyProps::vftable = __null; Sexy::RtClass* U46Project::ZombieRedBoyProps::s_rtClass = __null;;

void U46Project::ZombieRedBoyProps::modInit() 
{
	LOGI("[RedBoyProps::modInit]");
	vftable = copyVFTable(getActualOffset(ZOMBIEPROPS_VFTABLE), ZOMBIEPROPS_VFTABLE_FUNC_COUNT);
	LOGI("[RedBoyProps::modInit] Step 1");
	patchVFTable(vftable, (void*)ZombieRedBoyProps::getRTClass, RTOBJECT_GETTYPE_INDEX);
	LOGI("[RedBoyProps::modInit] Step 2");
	patchVFTable(vftable, (void*)ZombieRedBoyProps::DestroyAndDealloc, RTOBJECT_DESTRUCT_INDEX);
	LOGI("[RedBoyProps::modInit] Step 3");
	ZombieRedBoyProps::getRTClass();
	LOGI("[RedBoyProps::modInit] And done");
}
