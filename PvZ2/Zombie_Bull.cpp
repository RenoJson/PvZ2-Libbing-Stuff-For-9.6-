#include "Zombie_Bull.h"
#include "Bullrider.h"
#include "ZombieType.h"
#include "Zombie.h"
#include "ZombieConditions.h"
#include <atomic>
#include <mutex>
#include <assert.h>
#include <pch.h>

#pragma region Hide Custom Rider Layer 
typedef void(*zombieBullHideLayer)(Zombie*, int);
zombieBullHideLayer oZombieBullHideLayer = nullptr;
void HideCustomRiderLayer(Zombie* self, int animRig) {

    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());

    if (!props) {
        //LOGI("Props null, using original function instead");
        return oZombieBullHideLayer(self, animRig);
    }

    //LOGI("LayerCount: %zu", name.c_str(), props->RiderLayerToHide.size());

    if (self == 0) {
        return oZombieBullHideLayer(self, animRig);
    }

    typedef int (*setLayerVisibleFunc)(int, SexyString*, bool);
    setLayerVisibleFunc setLayerVisible = (setLayerVisibleFunc)getActualOffset(0x65B37C);

    if (!setLayerVisible) {
        //LOGI("setLayerVisible function not found");
        return oZombieBullHideLayer(self, animRig);
    }

    if (!props->RiderLayerToHide.empty()) {
        for (size_t i = 0; i < props->RiderLayerToHide.size(); i++) {
            const auto& layerStr = props->RiderLayerToHide[i];
            //LOGI("Hiding layer[%zu]: %s", i, layerStr.c_str());

            SexyString layerName(layerStr);
            setLayerVisible(animRig, &layerName, false);
        }
        //LOGI("Successfully hidden %zu layers", props->RiderLayerToHide.size());
    }
    else {
        // empty layer are making the game crash somehow, so this return is for placeholder
        LOGI("Empty layer array");
        return oZombieBullHideLayer(self, animRig);
    }
}
#pragma endregion

// thanks jay_krow for the codebase to help me recreate the function has more context
#pragma region hk Rider Type To Launch


typedef void(*zombieBullThrowRider)(Zombie*, int);
zombieBullThrowRider oZombieBullThrowRider = nullptr;

// these function is must have to make the bull work properly
using sub_736DE4_t = int(*)(int);
sub_736DE4_t sub_736DE4 = reinterpret_cast<sub_736DE4_t>(getActualOffset(UNKFUNC_SUB_736DE4));

using getboard_t = int(*)();
getboard_t getboard = reinterpret_cast<getboard_t>(getActualOffset(UNKFUNC_GET_BOARD));

void hkZombieBullThrowRider(Zombie* self, int a2)
{

    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    std::string name = props->RiderType;
    float distance = props->LandOffsetX;
    if (self == nullptr) {
        return oZombieBullThrowRider(self, a2);
    }
    typedef bool (*checkZombieHasCondition)(int, int);
    checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0x8A584C);

    if (!hasCondition((int)self, zombie_condition_shrinking) && !hasCondition((int)self, zombie_condition_shrunken)) 
    {
        // call when the lauch action frame are called to summon the zombie
        int getBullRiderAnimRig = sub_736DE4((int)self);
        // calling hiding layer after launch action frame (must match the bull animrig)
        HideCustomRiderLayer(self, getBullRiderAnimRig);
        // idk why using getBoard in the solution are make the game crash but using this and the game work fine
        int board = getboard();
        typedef int (*getZombieTypeDirectoryInstance)();
        int typeDir = ((getZombieTypeDirectoryInstance)getActualOffset(0x281008))();

        typedef void (*getZombieType)(Sexy::RtWeakPtr<int>*, int, std::string*);
        Sexy::RtWeakPtr<int> zType;
		
        ((getZombieType)getActualOffset(0x28107C))(&zType, typeDir, &name);

        // Spawn the custom rider i guess
        typedef int (*addZombieByType)(int, int, int, char, int);
        addZombieByType funAddZombieByType = (addZombieByType)getActualOffset(0x720E84);

        int spawnedRider = funAddZombieByType(board, (int)&zType, -1, 6, -1);

        typedef void (*dtor)(Sexy::RtWeakPtr<int>*);
        ((dtor)(getActualOffset(0x10C8B38)))(&zType);

        *(char*)((uintptr_t)spawnedRider + 0x30D) = true;
		
        if (hasCondition((int)self, zombie_condition_hypnotized)) {
		     typedef void (*setHypnoProperty)(int, int, int, int, int);
		     setHypnoProperty setHypno = (setHypnoProperty)getActualOffset(0x8A7EC8);
		     setHypno(spawnedRider, zombie_condition_hypnotized, 0x7F7FFFFF, 0, 0);
		     int bullProperty = *(int*)((uintptr_t)self + 0x20);
		     *(int*)((uintptr_t)spawnedRider + 0x20) = bullProperty;
		 }

        // Set rider position

        float newX = *(float*)((uintptr_t)self + 0x14) - 60;
        float newY = *(float*)((uintptr_t)self + 0x18);
        float newZ = *(float*)((uintptr_t)self + 0x1C) + 50;

        typedef int (*boardEntitySetPosition)(int, SexyVector3*);
        boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x2C9BAC);
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);

        funBoardEntitySetPosition(spawnedRider, &newCoords);
        typedef void (*ZombieThrowVirtual)(int, float, float, float, int, int, int); 

        // Get the launch virtual function in 0x75667C, this function is must have
        // Because if didn't have this, the bull only spawn the rider to the left instead launching 
        // But can only using imp class due to anim stuff 
        // Use Snowie action frame to transform the rider when the rider is landed if you don't want use imp 

        int* vtable = *(int**)spawnedRider; 
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[0x338 / 4]); 

        float targetX = *(float*)((uintptr_t)self + 0x14) - distance;
        float targetY = *(float*)((uintptr_t)self + 0x18); 
        float targetZ = *(float*)((uintptr_t)self + 0x1C); 

        virtualThrow(spawnedRider, targetX, targetY, targetZ, 0x3F400000, 0x437A0000, 0);
        // i didn't test on veteran bull yet because i'm lazy
        
        // maybe the bool here are for the veteran bull

        *(bool*)((uintptr_t)self + 0x308) = true;

    }
    else {
        LOGI("Condition exist");
    }
}
void hkCheckingTypename(Zombie* self, int a2)
{
    // Checking softcode typename before using hooking function
    auto* props = reinterpret_cast<ZombieBullProps*>(self->m_propertySheet.Get());
    if (self == 0) {
        return oZombieBullThrowRider(self, a2);
    }
    std::string name = props->RiderType;
    if (name == "west_bullrider") {
        return oZombieBullThrowRider(self, a2);
    }
    else {
        return hkZombieBullThrowRider(self, a2);
    }
}
#pragma endregion

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullProps::oZombieBullPropsBuildSymbols = nullptr;

void ZombieBullProps::modInit(){
	LOGI("init bull class");
	FluffyHookFunction(0x75667C, (void*)hkCheckingTypename, (void**)&oZombieBullThrowRider);
    FluffyHookFunction(0x756A98, (void*)HideCustomRiderLayer, (void**)&oZombieBullHideLayer);
    FluffyHookFunction(0xA0A7F4, (void*)construct, nullptr);
	LOGI("init bull class complete");
	LOGI("init bull props");
	FluffyHookFunction(0xA0A96C, (void*)ZombieBullProps::buildSymbols, (void**)&ZombieBullProps::oZombieBullPropsBuildSymbols);
	LOGI("init bull props complete");
	LOGI("finish init bull");
}
