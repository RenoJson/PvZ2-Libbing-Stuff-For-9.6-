#pragma once
#include "Sexy/RtWeakPtr.h"
#include "Sexy/DelegateBase.h"

class ZombieEvent
{
	Sexy::RtWeakPtr<int> ownerPtr;
	SexyString eventName;
	Sexy::DelegateBase delegate;
	int isDelegateSetup;
};

