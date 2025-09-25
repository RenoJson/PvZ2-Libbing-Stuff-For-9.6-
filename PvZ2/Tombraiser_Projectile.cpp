
#include <cstring>
#include <cstdlib>

#include "ProjectilePropertySheet.h"
#include "Projectile.h"
//0x317B64 is where the sheet base are stored
#pragma region Softcode Grid (ASM-matched)

using SceneCheckFunc = int(*)(int scene, int x, int y, int p1, int p2);
using SpriteCreateFunc = int(*)(int scene, void* params, int x, int y);
using ObjectConfigFunc = void(*)(int object, int param);
using ResourceManagerFunc = int(*)();
using AnimationCreateFunc = int(*)(int scene, int resource);
using BufferProcessFunc = int(*)(void* dest, void* src);
using ResourceLoadFunc = int(*)(int param);
using ResourceSetupFunc = void(*)(void* dest, int resource, void* src);
using BufferCheckFunc = bool(*)(void* param);
using BufferGetFunc = int(*)(void* param);
using AnimationGetFunc = int(*)(int resource, int param);
using AnimationInstanceFunc = int(*)(int param1, void* param2);
using BufferCleanupFunc = void(*)(int param1, int param2);
using AnimationSetFunc = void(*)(int object, int animation, int param);
using AnimationEnableFunc = void(*)(int object, int param);
using PositionSetFunc = void(*)(int object, float* position, int layer);
using AnimationNameSetFunc = void(*)(int object, void* animName, int param);
using SoundManagerFunc = int(*)(int param);
using SoundPlayFunc = void(*)(int soundSystem, const char* soundName, int param);

// Function pointers initialized with actual offsets
extern SceneCheckFunc        sub_716FA4;
extern SpriteCreateFunc      sub_716690;
extern ObjectConfigFunc      sub_E86C58;
extern ResourceManagerFunc   sub_E54A84;
extern AnimationCreateFunc   sub_716354;
extern BufferProcessFunc     sub_C9B7D4;
extern ResourceLoadFunc      sub_10D99A4;
extern ResourceSetupFunc     sub_10DFA5C;
extern BufferCheckFunc       sub_10C8C60;
extern BufferGetFunc         sub_10C8B64;
extern AnimationGetFunc      sub_10DF644;
extern AnimationInstanceFunc sub_10DB9C0;
extern BufferCleanupFunc     sub_10C8B38;
extern AnimationSetFunc      sub_E56188;
extern AnimationEnableFunc   sub_E56260;
extern PositionSetFunc       sub_735B60;
extern AnimationNameSetFunc  sub_E564A4;
extern SoundManagerFunc      sub_26AA10;
extern SoundPlayFunc         sub_6B3068;

extern int* dword_1D92A94; // Correctly typed as int*

SceneCheckFunc sub_716FA4 = (SceneCheckFunc)getActualOffset(0x716FA4);
SpriteCreateFunc sub_716690 = (SpriteCreateFunc)getActualOffset(0x716690);
ObjectConfigFunc sub_E86C58 = (ObjectConfigFunc)getActualOffset(0xE86C58);
ResourceManagerFunc sub_E54A84 = (ResourceManagerFunc)getActualOffset(0xE54A84);
AnimationCreateFunc sub_716354 = (AnimationCreateFunc)getActualOffset(0x716354);
BufferProcessFunc sub_C9B7D4 = (BufferProcessFunc)getActualOffset(0xC9B7D4);
ResourceLoadFunc sub_10D99A4 = (ResourceLoadFunc)getActualOffset(0x10D99A4);
ResourceSetupFunc sub_10DFA5C = (ResourceSetupFunc)getActualOffset(0x10DFA5C);
BufferCheckFunc sub_10C8C60 = (BufferCheckFunc)getActualOffset(0x10C8C60);
BufferGetFunc sub_10C8B64 = (BufferGetFunc)getActualOffset(0x10C8B64);
AnimationGetFunc sub_10DF644 = (AnimationGetFunc)getActualOffset(0x10DF644);
AnimationInstanceFunc sub_10DB9C0 = (AnimationInstanceFunc)getActualOffset(0x10DB9C0);
BufferCleanupFunc sub_10C8B38 = (BufferCleanupFunc)getActualOffset(0x10C8B38);
AnimationSetFunc sub_E56188 = (AnimationSetFunc)getActualOffset(0xE56188);
AnimationEnableFunc sub_E56260 = (AnimationEnableFunc)getActualOffset(0xE56260);
PositionSetFunc sub_735B60 = (PositionSetFunc)getActualOffset(0x735B60);
AnimationNameSetFunc sub_E564A4 = (AnimationNameSetFunc)getActualOffset(0xE564A4);
SoundManagerFunc sub_26AA10 = (SoundManagerFunc)getActualOffset(0x26AA10);
SoundPlayFunc sub_6B3068 = (SoundPlayFunc)getActualOffset(0x6B3068);

// biến toàn cục
int* dword_1D92A94 = reinterpret_cast<int*>(getActualOffset(0x1D92A94));
// String parameter structure matching ASM layout exactly
struct StringParam {
    int flags;   // Bit flags for memory management
    int length;  // String length  
    char* data;  // Pointer to string data
};

class GameObject1 {
public:
    virtual ~GameObject1() {}
    virtual int GetSomeValue() = 0;        // vtable offset +176 (0xB0)
    virtual int CallMethod164(int param) = 0;  // vtable offset +164 (0xA4)

    // These are at fixed offsets in the object
    char padding[0x140];  // Pad to correct offset
    int position_x;       // offset 0x140
    int position_y;       // offset 0x144
};

typedef void(*Bone)(Projectile*);
Bone oBone = nullptr;

// Helper to create StringParam with owned memory (like ASM does)
static inline StringParam CreateOwnedStringParam(const char* str, int allocSize) {
    StringParam sp{};
    sp.data = static_cast<char*>(operator new(allocSize)); // Match ASM: operator new
    sp.flags = allocSize + 1; // Set ownership bit + size info  
    sp.length = strlen(str);
    memcpy(sp.data, str, sp.length);
    sp.data[sp.length] = 0; // Null terminate
    return sp;
}

// Helper to cleanup StringParam matching ASM logic
static inline void CleanupStringParam(StringParam& sp) {
    if (sp.flags & 1) { // Check ownership bit
        operator delete(sp.data); // Match ASM: operator delete
        sp.data = nullptr;
        sp.flags = 0;
    }
}

// Main function matching ASM logic exactly
int hkBone(Projectile* self, GameObject1* gameObject)
{
    LOGI("hkBone: Starting gravestone creation");

    if (!self || !gameObject) {
        LOGI("hkBone: Invalid parameters");
        return 0;
    }

    // Get scene manager from global state (matches ASM offset 0x6C0)
    int sceneManagerBase = dword_1D92A94[0x6C0 / 4]; // Divide by 4 for int array indexing
    LOGI("hkBone: sceneManager=%p", (void*)sceneManagerBase);

    // Get positions from GameObject (matches ASM offsets 0x140, 0x144)
    int x = gameObject->position_x;
    int y = gameObject->position_y;
    LOGI("hkBone: position (%d, %d)", x, y);

    auto* props = reinterpret_cast<ProjectilePropertySheet*>(self->m_propertySheetPtr.Get());
    if (!props) {
        LOGI("hkBone: Missing property sheet, calling cleanup");
        // Match ASM: call vtable[0xA4] on gameObject  
        int* vtable = *reinterpret_cast<int**>(gameObject);
        if (!vtable) return 0; // Safety check
        auto cleanup_fn = reinterpret_cast<int(*)(GameObject1*, int)>(vtable[0xA4 / 4]);
        if (!cleanup_fn) return 0; // Safety check
        return cleanup_fn(gameObject, 0);
    }

    const char* gridtype_str = "gravestone_tutorial";
    const char* animname_str = props->ImpactPAM.c_str();

    LOGI("hkBone: Using gridtype=%s", gridtype_str);
    LOGI("hkBone: Using animname=%s", animname_str);

    // 1) Check if can create gravestone (matches ASM call to sub_716FA4)
    if (!sub_716FA4(sceneManagerBase, x, y, 0, 1)) {
        LOGI("hkBone: Position check failed, calling cleanup");
        int* vtable = *reinterpret_cast<int**>(gameObject);
        if (!vtable) return 0;
        auto cleanup_fn = reinterpret_cast<int(*)(GameObject1*, int)>(vtable[0xA4 / 4]);
        if (!cleanup_fn) return 0;
        return cleanup_fn(gameObject, 0);
    }

    // 2) Create gravestone sprite with owned StringParam (matches ASM allocation)
    StringParam gravestoneParam = CreateOwnedStringParam(gridtype_str, 0x20); // ASM uses 0x20 bytes

    int gravestoneSprite = sub_716690(sceneManagerBase, &gravestoneParam, x, y);
    LOGI("hkBone: Created gravestoneSprite=%p", (void*)gravestoneSprite);

    // Cleanup gravestone param (matches ASM)
    CleanupStringParam(gravestoneParam);

    if (!gravestoneSprite) {
        LOGI("hkBone: Failed to create gravestone sprite");
        int* vtable = *reinterpret_cast<int**>(gameObject);
        if (!vtable) return 0;
        auto cleanup_fn = reinterpret_cast<int(*)(GameObject1*, int)>(vtable[0xA4 / 4]);
        if (!cleanup_fn) return 0;
        return cleanup_fn(gameObject, 0);
    }

    // Configure gravestone (matches ASM)
    sub_E86C58(gravestoneSprite, 0);
    LOGI("hkBone: Gravestone configured");

    // Store position for later use (matches ASM local vars)
    int stored_x = x; // matches [SP,#0x48+var_44] 
    int stored_y = y; // R8 in ASM

    // 3) Create animation object (matches ASM sequence)
    int resMgr = sub_E54A84();
    int animObject = sub_716354(sceneManagerBase, resMgr);
    LOGI("hkBone: animObject=%p resMgr=%p", (void*)animObject, (void*)resMgr);

    // 4) Create animation resource with owned StringParam (matches ASM 0x40 byte allocation)
    StringParam animParam = CreateOwnedStringParam(animname_str, 0x40); // ASM uses 0x40 bytes

    // Animation processing (matches ASM buffer operations)
    alignas(8) unsigned char tempBuffer1[16]{}; // R9 in ASM, var_40
    alignas(8) unsigned char tempBuffer2[16]{}; // var_28

    int animInstance = 0;

    // Replace try-catch with null checks and validation
    if (animParam.data && animParam.length > 0) {
        int animResKey = sub_C9B7D4(tempBuffer1, &animParam);

        if (animResKey) {
            int resHandle = sub_10D99A4(animResKey);
            LOGI("hkBone: animResKey=%d resHandle=%d", animResKey, resHandle);

            if (resHandle) {
                sub_10DFA5C(tempBuffer2, resHandle, tempBuffer1);

                // Check if resource is valid (matches ASM logic)
                if (!sub_10C8C60(tempBuffer2)) {
                    int resData = sub_10C8B64(tempBuffer2);
                    if (resData) {
                        int animData = sub_10DF644(resHandle, resData);
                        LOGI("hkBone: resData=%d animData=%d", resData, animData);
                        if (animData) {
                            animInstance = sub_10DB9C0(animData, tempBuffer2);
                            LOGI("hkBone: Created animInstance=%d", animInstance);
                        }
                    }
                }
                else {
                    LOGI("hkBone: Buffer check failed, no animation instance");
                }
            }
        }
    }

    // Cleanup buffers (matches ASM cleanup sequence)
    sub_10C8B38(reinterpret_cast<int>(tempBuffer2), 0); // First cleanup
    if (animObject && animInstance) {
        sub_E56188(animObject, animInstance, 0);           // Set animation
    }
    sub_10C8B38(reinterpret_cast<int>(tempBuffer1), 232); // Second cleanup with 0xE8

    // Cleanup animation param
    CleanupStringParam(animParam);

    // 5) Enable animation (matches ASM)
    if (animObject) {
        sub_E56260(animObject, 1);
        LOGI("hkBone: Animation enabled");
    }

    // 6) Calculate positions exactly like ASM does
    // ASM: R5 = R1 + R2<<6  (R1=0xE8, R2=x)  
    // ASM: R7 = R0*0x4C + 0xC0 (R0=y)
    int pixel_x = 232 + (stored_x << 6);  // 0xE8 + x*64
    int pixel_y = (76 * stored_y) + 192;  // y*0x4C + 0xC0

    float position[3];
    position[0] = static_cast<float>(pixel_x);
    position[1] = static_cast<float>(pixel_y);
    position[2] = 0.0f; // Z always 0

    LOGI("hkBone: Calculated position (%.2f, %.2f, %.2f)", position[0], position[1], position[2]);

    // Get layer from vtable exactly like ASM (offset 0xB0 = 176)
    int layer = 2; // Default value
    if (gravestoneSprite) {
        int* vtable = *reinterpret_cast<int**>(gravestoneSprite);
        if (vtable) {
            auto layer_fn = reinterpret_cast<int(*)(int)>(vtable[0xB0 / 4]);
            if (layer_fn) {
                layer = layer_fn(gravestoneSprite) + 2; // ASM adds 2
                LOGI("hkBone: Got layer=%d", layer);
            }
            else {
                LOGI("hkBone: layer_fn is null, using default");
            }
        }
        else {
            LOGI("hkBone: vtable is null, using default layer");
        }
    }

    if (animObject) {
        sub_735B60(animObject, position, layer);
    }

    // 7) Set animation name exactly like ASM does
    if (animObject) {
        StringParam animNameParam{};
        const char* anim_key = "animation"; // matches aAnimation string in ASM

        // ASM stores this in a weird way - let's match it exactly
        animNameParam.flags = 0x12; // matches ASM
        animNameParam.length = 9;   // strlen("animation") 
        animNameParam.data = const_cast<char*>(anim_key);

        // Validate parameters before calling
        if (animNameParam.data && animNameParam.length > 0) {
            sub_E564A4(animObject, &animNameParam, 0);
            LOGI("hkBone: Animation name set to %s", anim_key);
        }
        else {
            LOGI("hkBone: Invalid animation name parameters");
        }
    }

    // 8) Play sound (matches ASM exactly)
    int soundSystem = sub_26AA10(0); // ASM passes 0 parameter  
    if (soundSystem) {
        sub_6B3068(soundSystem, "Play_Zomb_Egypt_TombRaiser_Grave_Rise", 0);
        LOGI("hkBone: Sound played, soundSystem=%p", (void*)soundSystem);
    }
    else {
        LOGI("hkBone: Failed to get sound system");
    }

    // 9) Final cleanup and return (matches ASM exactly)
    LOGI("hkBone: Calling gameObject vtable[0xA4]");
    int* vtable = *reinterpret_cast<int**>(gameObject);
    if (!vtable) return 0;
    auto final_fn = reinterpret_cast<int(*)(GameObject1*, int)>(vtable[0xA4 / 4]);
    if (!final_fn) return 0;
    return final_fn(gameObject, 0);
}
#pragma endregion

Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc ProjectilePropertySheet::oProjectilePropertySheetBuildSymbols = nullptr;
void ProjectilePropertySheet::modInit() {
    LOGI("init bone class");
    FluffyHookFunction(0x794628, (void*)hkBone, (void**)&oBone);
    LOGI("init bone class complete");
    LOGI("init bone props");
    FluffyHookFunction(0x317B64, (void*)ProjectilePropertySheet::buildSymbols, (void**)&ProjectilePropertySheet::oProjectilePropertySheetBuildSymbols);
    LOGI("init bone props complete");
    LOGI("finish init bone");
}