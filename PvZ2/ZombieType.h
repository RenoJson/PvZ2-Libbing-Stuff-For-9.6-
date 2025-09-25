#pragma once
#include <vector>

#include <PvZ2/ObjectTypeDescriptor.h>
#include <PvZ2/ZombiePropertySheet.h>
#include <Sexy/RtWeakPtr.h>

#define ZOMBIETYPEDIRECTORY_GET 0x281008
#define ZOMBIETYPEDIRECTORY_GETTYPE 0x28107C

enum ZombieFlagType {
	noflag = 0,
	flag_normal = 1,
	flag_veteran = 2
};

class ZombieType : public ObjectTypeDescriptor
{
public:
	SexyString ZombieClass;
	SexyString HomeWorld;
	SexyString DisplayTypeName;
	SexyString AlmanacBackdropName;
	SexyString PopAnim;
	SexyString AnimRigClass;
	std::vector<SexyString> ResourceGroups;
	std::vector<SexyString> AudioGroups;
	Sexy::RtWeakPtr<ZombiePropertySheet> Properties;
	bool Placeable;
	bool HastyOnStart;
	bool IsBasicZombie;
	bool IsFemaleZombie;
	bool HideFromAlmanac;
	ZombieFlagType FlagType;
	int unk;

	// new field
	int IntegerID = 0;

	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oZombieTypeBuildSymbols;
	static Reflection::CRefManualSymbolBuilder::ConstructFunc oZombieTypeConstruct;

	static void* construct(ZombieType* self)
	{
		oZombieTypeConstruct(self);

		self->IntegerID = 0;

		return self;
	}

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oZombieTypeBuildSymbols(builder, rclass);
		REGISTER_STANDARD_PROPERTY(builder, rclass, ZombieType, IntegerID);
	}

	static void GetFromName(Sexy::RtWeakPtr<ZombieType>* res, SexyString zombieName);
};

static_assert(sizeof(ZombieType) == 0x8C);
static_assert(offsetof(ZombieType, AudioGroups) == 0x64);
static_assert(offsetof(ZombieType, FlagType) == 0x80);