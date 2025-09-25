#pragma once
#include "Projectile.h"
#include "ProjectilePropertySheet.h"
// 794628 is the place where AE gravestone hardcode
// Why they are hardcode both the pam and gravestone????

class BoneProjectile: ProjectilePropertySheet 
{
public:
	static void modInit();
	SexyString GravestoneTypeToSpawn;
	static Reflection::CRefManualSymbolBuilder::BuildSymbolsFunc oProjectilePropertySheetBuildSymbols;

	static void buildSymbols(Reflection::CRefManualSymbolBuilder* builder, Reflection::RClass* rclass)
	{
		oProjectilePropertySheetBuildSymbols(builder, rclass);
		REGISTER_STRING_PROPERTY(builder, rclass, ProjectilePropertySheet, GravestoneTypeToSpawn);
	};
};
