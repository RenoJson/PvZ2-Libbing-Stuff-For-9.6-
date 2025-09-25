#pragma once
#include "PopAnimRig.h"
#include "ZombieType.h"
#include <Sexy/RtReflectionDelegateBase.h>

#define ZOMBIEANIMRIG_DTOR_ADDR 0x28AF30

class ZombieAnimRig : public PopAnimRig
{
public:
	SexyString m_groundTrackName;
	float m_lastGroundTrackOffset;
	float m_desiredVelocity;
	float m_animRateModifier;
	float m_needsToDieRequestedTime;
	float m_readyToDie;
	Sexy::RtWeakPtr<ZombieType> m_zombieType;

	virtual void LoopIdleWithCallback(Sexy::RtReflectionDelegateBase*) {}
	virtual void LoopWalkWithCallback(Sexy::RtReflectionDelegateBase*) {}
	virtual void Function34() {};
	virtual void Function35() {};
	virtual void Function36() {};
	virtual void Function37() {};
	virtual void Function38() {};
	virtual void PlayAnimWithCallback(SexyString* animName, int a3, Sexy::RtReflectionDelegateBase* a4) {};
	virtual void DropHead() {};
	virtual void Function41() {};
	virtual void Function42() {};
	virtual void Function43() {};
	virtual void Function44() {};
	virtual void DropArm() {};
	virtual void Function46() {};
	virtual void Function47() {};
	virtual void Function48() {};
	virtual void Function49() {};
	virtual void Function50() {};
	virtual void Function51() {};
	virtual void Function52() {};
	virtual void Function53() {};
	virtual void Function54() {};
	virtual void GetHeadLayerToDrop() {};
	virtual void GetArmLayerToDrop() {};
	virtual void Function57() {};
	virtual void Function58() {};
	virtual void Function59() {};
	virtual void Function60() {};
	virtual void Function61() {};
	virtual void Function62() {};
	virtual void Function63() {};
	virtual void Function64() {};
	virtual void Function65() {};
	virtual void Function66() {};
};

static_assert(sizeof(ZombieAnimRig) == 0x198);
static_assert(offsetof(ZombieAnimRig, m_groundTrackName) == 0x170);