#include "Chairracer.h"
#include "ZombieType.h"
#include "ZombieConditions.h"
#include <atomic>
#include <mutex>
#include <assert.h>
#include <pch.h>

#pragma region hk Racer Type To Launch

typedef void(*zombieChairThrowRacer)(ZombieZcorpRacer*, int);
zombieChairThrowRacer oZombieChairThrowRacer = nullptr;

void hkZombieChairThrowRacer(ZombieZcorpRacer* self, int a2)
{
    auto* props = reinterpret_cast<ZombieZcorpRacerProps*>(self->m_propertySheet.Get());
    std::string racerType = props->RacerType;

    if (self == nullptr) {
        return oZombieChairThrowRacer(self, a2);
    }

    typedef bool (*checkZombieHasCondition)(int, int);
    checkZombieHasCondition hasCond = (checkZombieHasCondition)getActualOffset(0x8A584C);

    if (!*(bool*)((uintptr_t)self + 0x308)) {  // 776 = 0x308 

        typedef int (*getChairRacerZombieAnimRig)(ZombieZcorpRacer*);
        int animRig = ((getChairRacerZombieAnimRig)getActualOffset(0x85A69C))(self);
        float animValue = *(float*)(animRig + 428);

        // Get board instance
        typedef int (*Board)();
        int board = ((Board)getActualOffset(0x949EFC))();

        // Get zombie type directory
        typedef int (*getZombieTypeDirectoryInstance)();
        int typeDir = ((getZombieTypeDirectoryInstance)getActualOffset(0x281008))();

        typedef void (*getZombieType)(Sexy::RtWeakPtr<int>*, int, std::string*);
        Sexy::RtWeakPtr<int> zType;

        if (hasCond((uintptr_t)self, zombie_condition_hypnotized)) {
           racerType = "hypno_" + racerType;
        }

        ((getZombieType)getActualOffset(0x28107C))(&zType, typeDir, &racerType);

        typedef int (*addZombieByType)(int, int, int, char, int);
        addZombieByType funAddZombieByType = (addZombieByType)getActualOffset(0x720E84);
        int spawnedRider = funAddZombieByType(board, (int)&zType, -1, 6, -1);

        // Clean up zombie type reference
        typedef void (*dtor)(Sexy::RtWeakPtr<int>*);
        ((dtor)(getActualOffset(0x10C8B38)))(&zType);

        //Racer zombie have weird landing animation
        //This bool flag should make the racer have proper landing animation
        *(char*)((uintptr_t)spawnedRider + 0x30D) = true;

        if (hasCond((uintptr_t)self, 43) || hasCond((uintptr_t)self, 44)) {
            typedef void (*setHypnoProperty)(int, int, int, int, int);
            setHypnoProperty setHypno = (setHypnoProperty)getActualOffset(0x8A7EC8);
            setHypno(spawnedRider, 44, 0x7F7FFFFF, 0, 0);
        }

        float launchDistance = animValue * 64.0f;

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
        // skip hypno checking for now
        bool isHypnotized = hasCond((uintptr_t)self, zombie_condition_hypnotized);

        if (isHypnotized) {
            // Set hypno properties
            typedef void (*setHypnoProperty)(int, int, int, int, int);
            setHypnoProperty setHypno = (setHypnoProperty)getActualOffset(0x8A7EC8);
            setHypno(spawnedRider, 23, 0x7F7FFFFF, 0, 0);

            // Copy property sheet (sub_CEB640)
            typedef void (*sub_CEB640_t)(int, int);
            sub_CEB640_t sub_CEB640 = (sub_CEB640_t)getActualOffset(0xCEB640);
            sub_CEB640(spawnedRider, *(int*)((uintptr_t)self + 0x20)); // offset 32 = 0x20

            // Get some value (v12)
            typedef int (*sub_8A5A8C_t)(int);
            sub_8A5A8C_t sub_8A5A8C = (sub_8A5A8C_t)getActualOffset(0x8A5A8C);
            int v12 = sub_8A5A8C((uintptr_t)self);

            // Call sub_8A8450
            typedef void (*sub_8A8450_t)(int, int);
            sub_8A8450_t sub_8A8450 = (sub_8A8450_t)getActualOffset(0x8A8450);
            sub_8A8450(spawnedRider, v12);

            // Calculate target position for hypnotized zombie (forward launch)
            targetX = launchDistance + currentX; 
            if (targetX > 776.0f) {
                targetX = 776.0f;
            }
        }
        else {
            // Calculate target position for normal zombie (backward launch)  
            
            if (targetX < 232.0f) {
                targetX = 232.0f;
            }
        }

        typedef void (*ZombieThrowVirtual)(int, float, float, float, int, int, int);
        int* vtable2 = *(int**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable2[0x338 / 4]);

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

void ZombieZcorpRacer::modInit() {
    LOGI("init chair class");
    FluffyHookFunction(0x85A7F0, (void*)hkZombieChairThrowRacer, (void**)&oZombieChairThrowRacer);
    FluffyHookFunction(0x8594B0, (void*)ZombieZcorpRacerProps::construct, nullptr);
    LOGI("init chair class complete");
    LOGI("init chair props");
    FluffyHookFunction(0x859628, (void*)ZombieZcorpRacerProps::buildSymbols, (void**)&ZombieZcorpRacerProps::oZombieZcorpRacerPropsBuildSymbols);
    LOGI("init chair props complete");
    LOGI("finish chair init");
}
