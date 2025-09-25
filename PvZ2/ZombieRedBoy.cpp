#include "ZombieRedBoy.h"
// this is only put the class into the game, i'm not patching its function yet
DEFINE_STATIC_RT_CLASS_MEMBERS(U46Project::ZombieRedBoy);

void U46Project::ZombieRedBoy::modInit()
{
	LOGI("[RedBoy::modInit]");
	vftable = copyVFTable(getActualOffset(ZOMBIE_VFTABLE_ADDR), ZOMBIE_VFUNC_COUNT);
	LOGI("[RedBoy::modInit] Step 1");
	patchVFTable(vftable, (void*)ZombieRedBoy::getRTClass, RTOBJECT_GETTYPE_INDEX);
	LOGI("[RedBoy::modInit] Step 2");
	patchVFTable(vftable, (void*)ZombieRedBoy::DestroyAndDealloc, RTOBJECT_DESTRUCT_INDEX);
	LOGI("[RedBoy::modInit] Step 3");
	ZombieRedBoy::getRTClass();
	LOGI("[RedBoy::modInit] And done");
}
