#pragma once
//We need bullrider props to get the launch property
#include "ZombiePropertySheet.h"
#include "Zombie.h"
class ZombieImp : public Zombie {
public:
	static void modInit();};
class ZombieBullRiderProps : public ZombiePropertySheet
{
public:
	float LaunchAirTime;
	float LaunchDistance;
	float LaunchHeight;
};
