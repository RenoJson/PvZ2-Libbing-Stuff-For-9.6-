#pragma once
#include "memUtils.h"

void DisableProgressMeterMsg();

void SurvivalLevelEndedMsg();

enum MessageId {
	PlantBoosted = 0xD04218,
	PressViewBoardOrZombies = 0xF75904,
	WaveManagerUpdateToPercent = 0x29E3E0,
	SetFlagCount = 0x55ADC0,
	DisableProgressMeter = -1,
	SurvivalLevelEnded = -2,
	NextWaveProgressMsg = 0x55ADB0,
	//LevelStartMsg = 0x2ACC78 // 9.4
};

class Messages {
public:
	static uint Get(MessageId id) {
		if ((uint)id < 0) {
			switch (id) {
			case DisableProgressMeter:
				return (uint)DisableProgressMeterMsg;
			case SurvivalLevelEnded:
				return (uint)SurvivalLevelEndedMsg;
			}
		}
		return getActualOffset((uint)id);
	}
};
