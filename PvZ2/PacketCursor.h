#pragma once

#include "Sexy/RtObject.h"
#include "Reflection/ReflectionBuilder.h"
#include "Sexy/RtWeakPtr.h"

class BaseCursor : public Sexy::RtObject {};

class PacketCursor : public BaseCursor {
public:
	int mX;
	int mY;
	Rect bounds;
	char pad001C[0xC];
	Sexy::RtWeakPtr<void> unkPtr;

	virtual void Function7() {};
	virtual void Function8() {};
	virtual void Function9() {};
	virtual void Function10() {};
	virtual void Function11() {};
	virtual bool CanPlantOnCursor() { return true; }; // stub implementation
	virtual void GetGridCoords() {}; // cannot accurately recreate atm i think
	virtual void AttemptPlant() {};

	static Sexy::RtClass* getRtClass() {
		typedef Sexy::RtClass* (*getType)(); getType pGetType = (getType)getActualOffset(0x929E00); return pGetType();
	}; // 9.4
};

