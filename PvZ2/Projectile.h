#pragma once
#include "ModularRealObject.h"
#include "BoardEntity.h"
#include "DamageInfo.h"
#include "ProjectilePropertySheet.h"
#include "AttachedEffectManager.h"

#define OFFSET_PROJECTILE_PLAY_IMPACT 0x907988

class Projectile : public ModularRealObject
{
public:
	bool m_shouldAffectTarget; //0x0038
	bool m_unk0x39; //0x0039
	bool m_unk0x3A; //0x003A
	bool m_unk0x3B; //0x003B
	void* m_propertySheet; //0x003C
	Sexy::RtWeakPtr<ProjectilePropertySheet> m_propertySheetPtr; //0x0040
	Sexy::RtWeakPtr<RtObject> m_instigator; //0x0048
	int32_t m_instigatorLevel; //0x0050
	int32_t m_instigatorPowerUpFlags; //0x0054
	Sexy::RtWeakPtr<RtObject> m_instigatorType; //0x0058
	float m_timeStepOverride; //0x0060
	SexyVector3 m_previousPosition; //0x0064
	SexyVector3 m_velocity; //0x0070
	SexyVector3 m_acceleration; //0x007C
	SexyVector3 m_velocityScale; //0x0088
	int32_t m_boardRow; //0x0094
	int32_t m_previousBoardRow; //0x0098
	int32_t m_boardRowHeight; //0x009C
	int32_t m_boardRowFiredOrDeflectedFrom; //0x00A0
	int m_unk0xA4; //0x00A4
	Sexy::RtWeakPtr<RtObject> m_renderImage; //0x00A8
	int32_t m_renderOrder; //0x00B0
	float m_scale; //0x00B4
	float m_rotation; //0x00B8
	float m_baseRotation; //0x00BC
	float m_angularVelocity; //0x00C0
	int m_unk0xC4; //0x00C4
	bool m_mirrored; //0x00C8
	bool m_collideLeftToRight; //0x00C9
	char pad_00CA[2]; //0x00CA
	float m_damageAmount; //0x00CC
	float m_splashDamageAmount; //0x00D0
	float m_stunDurationAmount; //0x00D4
	int64_t m_damageFlags; //0x00D8
	std::vector<int> m_damageConditions; //0x00E0
	int32_t m_collisionFlagsOverride; //0x00EC
	int32_t m_propertyFlags; //0x00F0
	char pad_00F4[4]; //0x00F4
	Sexy::RtWeakPtr<RtObject> m_attachedAnimRig; //0x00F8
	class SexyVector2 m_attachedAnimRigOffset; //0x0100
	Sexy::RtWeakPtr<RtObject> m_target; //0x0108
	char pad_0110[24]; //0x0110
	std::vector<SexyString> m_plantFamilies; //0x0128
	float m_resistancePiercingOverride; //0x0134
	int32_t m_plantTier; //0x0138

	template<typename T>
	T* GetProps()
	{
		if (m_propertySheet)
			return reinterpret_cast<T*>(m_propertySheet);

		auto* props = m_propertySheetPtr.Get();
		m_propertySheet = props;
		return reinterpret_cast<T*>(props);
	}

	void PlayImpact(BoardEntity* target) {
		typedef void (*PlayImpactFunc)(Projectile*, BoardEntity*);
		((PlayImpactFunc)getActualOffset(OFFSET_PROJECTILE_PLAY_IMPACT))(this, target);
	}

	virtual bool CollideWithObject(GameObject* object) {};
	virtual void OnCollision() {};
	virtual void Function37() {};
	virtual void Function38() {};
	virtual void Function39() {};
	virtual void UpdateVelocity() {};
	virtual void Function41() {};
	virtual void Function42() {};
	virtual void Function43() {};
	virtual void Function44() {};
	virtual void Function45() {};
	virtual void Function46() {};
	virtual void AffectTarget(BoardEntity* target) {};
	virtual void Function48() {};
	virtual void Function49() {};
	virtual void GetDamageInfo(DamageInfo* outDmgInfo) {};
	virtual void FindCollisionTargets(std::vector<BoardEntity*>& targets) {};
	virtual void Function52() {};
};