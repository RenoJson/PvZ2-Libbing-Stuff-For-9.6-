#pragma once
#include "Sexy/RtObject.h"
#include "Plant.h"
#include "PopAnimRig.h"
#include "Zombie.h"

class PlantSpikeweed : public PlantFramework {
public:
	std::vector<Sexy::RtWeakPtr<PopAnimRig>> m_plantfoodSpikes;
	bool m_plantfoodSpikesActive;
	std::vector<Sexy::RtWeakPtr<Zombie>> m_zombiesBeingPulled;
};

class PlantSpikerock : public PlantSpikeweed {
public:
	int m_remainingSmashes;
};

static_assert(offsetof(PlantSpikerock, m_remainingSmashes) == 0x24);