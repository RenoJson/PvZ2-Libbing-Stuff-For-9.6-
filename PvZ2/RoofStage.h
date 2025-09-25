#pragma once
#include "StageModule.h"
#include "PvZ2/RoofStageProps.h"
#include "PvZ2/Board.h"

class RoofStage : public StageModule
{
	RoofStageProps* props = NULL;

	DECLARE_STATIC_RT_CLASS_MEMBERS(RoofStage);

	DEFINE_RT_CLASS_BUILD_SYMBOLS_WITHOUT_CLASS_PROPERTIES(RoofStage, StageModule);

	static void* construct() {
		((void)__android_log_print(ANDROID_LOG_INFO, "PVZ2ExampleMod", "Constructing ""RoofStage"));
		auto* instance = new RoofStage();
		typedef void* (*ctorWithThisPtr)(void*);
		ctorWithThisPtr baseCtor = (ctorWithThisPtr)getActualOffset(0x4B26F8);
		baseCtor(instance);

		instance->props = NULL;

		setVFTable(instance, (uintptr_t)vftable);
		return instance;
	};

	DEFINE_RT_CLASS_GET_CLASS_FUNCTION(RoofStage, STAGEMODULE_GETCLASS_ADDR);

	DEFINE_RT_CLASS_REGISTER_CLASS_FUNCTION(RoofStage);

	static RoofStage* Get() {
		auto stageModule = StageModule::GetInstance(getBoard());

		if (stageModule->IsType(RoofStage::getRtClass())) {
			return (RoofStage*)stageModule;
		}
		else {
			return NULL;
		}
	}

	inline RoofStageProps* GetProps() {
		if (!props) {
			auto voidProps = this->m_propertySheetPtr.Get();
			props = (RoofStageProps*)voidProps;
		}

		return props;
	}

	float GetDepth(float mX, float mY) {
		int col = -1;
		if (mX >= GRID_TILE_LEFT) {
			col = (mX - GRID_TILE_LEFT) / GRID_TILE_WIDTH;
		}
		auto props = GetProps();
		int colCount = getBoard()->m_columnCount;
		// assuming ColumnHeights and KillHeights both have 10 elements. Will crash if this is not met
		if (col >= colCount) {
			return props->ColumnHeights[colCount];
		}
		else if (col < 0) {
			return props->ColumnHeights[0];
		}
		else {
			int leftHeight = props->ColumnHeights[col];
			int rightHeight = props->ColumnHeights[col + 1];
			float dist = mX - col * GRID_TILE_WIDTH - GRID_TILE_LEFT;
			return leftHeight + (float(rightHeight - leftHeight)) / GRID_TILE_WIDTH * dist;
		}
	}

	float GetKillDepth(float mX, float mY) {
		int col = -1;
		if (mX >= GRID_TILE_LEFT) {
			col = (mX - GRID_TILE_LEFT) / GRID_TILE_WIDTH;
		}
		auto props = GetProps();
		// assuming ColumnHeights and KillHeights both have 10 elements. Will crash if this is not met
		int colCount = getBoard()->m_columnCount;
		if (col >= colCount) {
			return props->KillHeights[colCount];
		}
		else if (col < 0) {
			return props->KillHeights[0];
		}
		else {
			int leftHeight = props->KillHeights[col];
			int rightHeight = props->KillHeights[col + 1];
			float dist = mX - col * GRID_TILE_WIDTH - GRID_TILE_LEFT;
			return leftHeight + (float(rightHeight - leftHeight)) / GRID_TILE_WIDTH * dist;
		}
	}

	void GetHeights(int* res) {
		auto props = GetProps();
		// LOGI("props colum heights = %d", props->ColumnHeights.size());
		for (int i = 0; i <= getBoard()->m_columnCount; ++i) {
			res[i] = props->ColumnHeights[i];
		}
	}
};

float GetSlopeOffset(float mX, float mY);
