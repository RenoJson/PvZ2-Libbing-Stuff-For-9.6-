#include "Zombie_Bull.h"
#include "Zombie_BullVeteran.h"
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

    // this one is for the imp that using the same sprite as wild west imp
    // we won't need using RiderLayerToHide symbol if the layer are the same as original wild west imp
    std::vector<SexyString> HardcodedLayer = {
         "zombie_cowboy_hat_back",
         "zombie_cowboy_hat_front",
         "zombie_imp_arm_inner_lower",
         "zombie_imp_arm_inner_upper",
         "zombie_imp_arm_outer_lower",
         "zombie_imp_arm_outer_upper_01",
         "zombie_imp_arm_outer_upper_02",
         "zombie_imp_arms_outer_upper",
         "zombie_imp_eye",
         "zombie_imp_eye_sm",
         "zombie_imp_hand_inner",
         "zombie_imp_hand_outer",
         "zombie_imp_jaw",
         "zombie_imp_leg_inner_lower",
         "zombie_imp_leg_inner_upper",
         "zombie_imp_leg_outer_lower",
         "zombie_imp_leg_outer_upper",
         "zombie_imp_pupil",
         "zombie_imp_skull",
         "zombie_imp_toe_inner",
         "zombie_imp_toe_outer",
         "zombie_imp_torso",
         "zombie_imp_waist",
         "zombie_imp_leg_outer_upper",
         "_zombie_imp_head_top"
    };
    typedef int (*setLayerVisibleFunc)(int, SexyString*, bool);
    setLayerVisibleFunc setLayerVisible = (setLayerVisibleFunc)getActualOffset(0x65B37C);

    if (!props->RiderLayerToHide.empty()) {
        for (size_t i = 0; i < props->RiderLayerToHide.size(); i++) {
            const auto& layerStr = props->RiderLayerToHide[i];
            SexyString layerName(layerStr);
            setLayerVisible(animRig, &layerName, false);
        }
    }
    else {
        for (size_t i = 0; i < HardcodedLayer.size(); i++) {
            const auto& layerStr = HardcodedLayer[i];
            SexyString layerName(layerStr);
            setLayerVisible(animRig, &layerName, false);
        }
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

    if (!props) {
        return oZombieBullThrowRider(self, a2);
    }

    // check is using BullVeteranProps or not
    int* vtable = *reinterpret_cast<int**>(props);
    bool isVeteran = (vtable == reinterpret_cast<int*>(getActualOffset(0x1CB8D60)));

    // using custom symbol depend on what props the bull use
    std::string name;
    float distance;

    if (isVeteran) {
        auto* veteranProps = static_cast<ZombieBullVeteranProps*>(props);
        name = veteranProps->VetRiderType;
        distance = veteranProps->LaunchDistance;
    }
    else {
        name = props->RiderType;
        distance = props->LaunchDistance;
    }
    // we won't need to use RiderType anymore if the zombie spawn from the bull is wild west imp
    if (name.empty()) {
        name = "west_bullrider";
    }

    typedef bool (*checkZombieHasCondition)(int, int);
    checkZombieHasCondition hasCondition = (checkZombieHasCondition)getActualOffset(0x8A584C);

    bool isShrinking = hasCondition((int)self, zombie_condition_shrinking);
    bool isShrunken = hasCondition((int)self, zombie_condition_shrunken);

    if (!*(bool*)((uintptr_t)self + 0x308) && !isShrinking && !isShrunken)
    {
        int getBullRiderAnimRig = sub_736DE4((int)self);
        HideCustomRiderLayer(self, getBullRiderAnimRig);
        int board = getboard();

        typedef int (*getZombieTypeDirectoryInstance)();
        int typeDir = ((getZombieTypeDirectoryInstance)getActualOffset(0x281008))();

        typedef void (*getZombieType)(Sexy::RtWeakPtr<int>*, int, std::string*);
        Sexy::RtWeakPtr<int> zType;

        ((getZombieType)getActualOffset(0x28107C))(&zType, typeDir, &name);

        typedef int (*addZombieByType)(int, int, int, char, int);
        addZombieByType funAddZombieByType = (addZombieByType)getActualOffset(0x720E84);

        int spawnedRider = funAddZombieByType(board, (int)&zType, -1, 6, -1);

        typedef void (*dtor)(Sexy::RtWeakPtr<int>*);
        ((dtor)(getActualOffset(0x10C8B38)))(&zType);

        *(char*)((uintptr_t)spawnedRider + 0x30D) = true;

        float newX = *(float*)((uintptr_t)self + 0x14) - 60;
        float newY = *(float*)((uintptr_t)self + 0x18);
        float newZ = *(float*)((uintptr_t)self + 0x1C) + 50;

        typedef int (*boardEntitySetPosition)(int, SexyVector3*);
        boardEntitySetPosition funBoardEntitySetPosition = (boardEntitySetPosition)getActualOffset(0x2C9BAC);
        SexyVector3 newCoords = SexyVector3(newX, newY, newZ);
        funBoardEntitySetPosition(spawnedRider, &newCoords);

        typedef void (*ZombieThrowVirtual)(int, float, float, float, int, int, int);
        int* vtable = *(int**)spawnedRider;
        ZombieThrowVirtual virtualThrow = (ZombieThrowVirtual)(vtable[0x338 / 4]);

        float targetX, targetY, targetZ;

        bool isHypnotized = hasCondition((int)self, zombie_condition_hypnotized);

        if (isHypnotized) {
            typedef void (*setConditionZ)(int, int, int, int, int);
            setConditionZ setCondition = (setConditionZ)getActualOffset(0x8A7EC8);
            setCondition(spawnedRider, zombie_condition_hypnotized, 0x7F7FFFFF, 0, 0);

            int bullProperty = *(int*)((uintptr_t)self + 0x20);
            *(int*)((uintptr_t)spawnedRider + 0x20) = bullProperty;

            targetX = *(float*)((uintptr_t)self + 0x14) + distance;
            targetY = *(float*)((uintptr_t)self + 0x18);
            targetZ = *(float*)((uintptr_t)self + 0x1C);

            if (targetX > 776.0f) {

                targetX = 776.0f;
            }
        }
        else {
            targetX = *(float*)((uintptr_t)self + 0x14) - distance;
            targetY = *(float*)((uintptr_t)self + 0x18);
            targetZ = *(float*)((uintptr_t)self + 0x1C);

            if (targetX < 232.0f) {
                targetX = 232.0f;
            }
        }

        virtualThrow(spawnedRider, targetX, targetY, targetZ, 0x3F400000, 0x437A0000, 0);

        // this one is prevent veteran bull throw further imp
        *(bool*)((uintptr_t)self + 0x308) = true;
    }
    else {
        LOGI("Already shrinking, shrunken or being thrown");
    }
}
typedef void (*zombieBullPlayDeath)(Zombie*);
zombieBullPlayDeath oZombieBullPlayDeath = nullptr;

void hkZombieBullPlayDeath(Zombie* self) {

    typedef void(*zombieFun197)(Zombie*);
    ((zombieFun197)getActualOffset(0x8B8330))(self); // does not play the bull's death sound effect
}
#pragma endregion

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ZombieBullProps::oZombieBullPropsBuildSymbols = nullptr;

void ZombieBullProps::modInit() {
    LOGI("init bull class");
    FluffyHookFunction(0x75667C, (void*)hkZombieBullThrowRider, (void**)&oZombieBullThrowRider);
    FluffyHookFunction(0x756A98, (void*)HideCustomRiderLayer, (void**)&oZombieBullHideLayer);
    FluffyHookFunction(0x755144, (void*)hkZombieBullPlayDeath, (void**)&oZombieBullPlayDeath);
    FluffyHookFunction(0xA0A7F4, (void*)construct, nullptr);
    LOGI("init bull class complete");
    LOGI("init bull props");
    FluffyHookFunction(0xA0A96C, (void*)ZombieBullProps::buildSymbols, (void**)&ZombieBullProps::oZombieBullPropsBuildSymbols);
    LOGI("init bull props complete");
    LOGI("finish init bull");
}