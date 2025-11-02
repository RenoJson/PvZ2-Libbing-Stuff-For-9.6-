#include "Chairracer.h"
#include "ZombieType.h"
#include "ZombieConditions.h"
#include <atomic>
#include <mutex>
#include <assert.h>
#include <pch.h>

#pragma region hk Racer Type To Launch

typedef void(*zombieChairThrowRacer)(Zombie*, int);
zombieChairThrowRacer oZombieChairThrowRacer = nullptr;

void hkZombieChairThrowRacer(Zombie* self, int a2)
{
    auto* props = reinterpret_cast<ZombieZcorpRacerProps*>(self->m_propertySheet.Get());
    std::string racerType = props->RacerType;

    if (racerType.empty()) {
        racerType = "zcorp_racer";
    }

    typedef bool (*checkZombieHasCondition)(int, int);
    checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0x8A584C);

    typedef void (*setConditionZ)(int, int, int, int, int);
    setConditionZ setCondition = (setConditionZ)getActualOffset(0x8A7EC8);

    if (!*(bool*)((uintptr_t)self + 0x308)) {  // 776 = 0x308 

        typedef int (*getChairRacerZombieAnimRig)(Zombie*);
        int animRig = ((getChairRacerZombieAnimRig)getActualOffset(0x85A69C))(self);
        float tileDistance = *(float*)(animRig + 428); // this one will using the LaunchZombieDistance from ZcorpRacer props

        // Get board instance
        typedef int (*Board)();
        int board = ((Board)getActualOffset(0x949EFC))();

        // Get zombie type directory
        typedef int (*getZombieTypeDirectoryInstance)();
        int typeDir = ((getZombieTypeDirectoryInstance)getActualOffset(0x281008))();

        typedef void (*getZombieType)(Sexy::RtWeakPtr<int>*, int, std::string*);
        Sexy::RtWeakPtr<int> zType;

        ((getZombieType)getActualOffset(0x28107C))(&zType, typeDir, &racerType);

        typedef int (*addZombieByType)(int, int, int, char, int);
        addZombieByType funAddZombieByType = (addZombieByType)getActualOffset(0x720E84);
        int spawnedRider = funAddZombieByType(board, (int)&zType, -1, 6, -1);

        // Clean up zombie type reference
        typedef void (*dtor)(Sexy::RtWeakPtr<int>*);
        ((dtor)(getActualOffset(0x10C8B38)))(&zType);

        *(char*)((uintptr_t)spawnedRider + 0x30D) = true;

        if (hasCondition((int)self, zombie_condition_shrinking) || hasCondition((int)self, zombie_condition_shrunken)) {

            setCondition(spawnedRider, zombie_condition_shrunken, 0x7F7FFFFF, 0, 0);
        }


        float launchDistance = tileDistance * 64.0f;

        float currentX = *(float*)((uintptr_t)self + 0x14);
        float currentY = *(float*)((uintptr_t)self + 0x18);
        float currentZ = *(float*)((uintptr_t)self + 0x1C);

        // Set initial position
        typedef int (*boardEntitySetPosition)(int, SexyVector3*);
        boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x2C9BAC);
        SexyVector3 position = SexyVector3(currentX, currentY, currentZ);
        funBoardEntitySetPosition(spawnedRider, &position);

        float targetX = currentX - launchDistance;
        int* vtable = *(int**)spawnedRider;
        typedef void (*VirtualFunc244)(int);
        VirtualFunc244 virtualFunc = (VirtualFunc244)(vtable[0xF4 / 4]);
        virtualFunc(spawnedRider);

        if (hasCondition((int)self, zombie_condition_hypnotized)) {

            setCondition(spawnedRider, zombie_condition_hypnotized, 0x7F7FFFFF, 0, 0);
            int bullProperty = *(int*)((uintptr_t)self + 0x20);
            *(int*)((uintptr_t)spawnedRider + 0x20) = bullProperty;

            targetX = launchDistance + currentX;
            if (targetX > 776.0f) {
                targetX = 776.0f;
            }
        }

        else {
            if (targetX < 232.0f) {
                targetX = 232.0f;
            }
        }

        typedef void (*ZombieThrowVirtual)(int, float, float, float, int, int, int);
        int* vtable2 = *(int**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable2[0x338 / 4]);

        // Get force parameters from animation rig (v2)
        int forceParam1 = *(int*)(animRig + 0x1B4);
        int forceParam2 = *(int*)(animRig + 0x1B0);

        // Execute the throw/launch
        virtualThrow(spawnedRider, targetX, currentY, currentZ, forceParam1, forceParam2, 0);

        // Mark as launched
        *(bool*)((uintptr_t)self + 0x308) = true; // 776 = 0x308
    }
}

#pragma endregion

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieZcorpRacerProps::oZombieZcorpRacerPropsBuildSymbols = nullptr;

void ZombieZcorpRacerProps::modInit() {
    LOGI("init chair class");
    FluffyHookFunction(0x85A7F0, (void*)hkZombieChairThrowRacer, (void**)&oZombieChairThrowRacer);
    FluffyHookFunction(0x8594B0, (void*)ZombieZcorpRacerProps::construct, nullptr);
    LOGI("init chair class complete");
    LOGI("init chair props");
    FluffyHookFunction(0x859628, (void*)ZombieZcorpRacerProps::buildSymbols, (void**)&ZombieZcorpRacerProps::oZombieZcorpRacerPropsBuildSymbols);
    //FluffyHookFunction(0x859628, (void*)ZombieZcorpRacerProps::buildSymbols, nullptr);
    LOGI("init chair props complete");
    LOGI("finish chair init");
}
