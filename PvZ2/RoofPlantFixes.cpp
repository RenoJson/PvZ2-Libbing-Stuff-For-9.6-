#include "pch.h"
#include "PvZ2/RoofPlantFixes.h"
#include "PlantSpikeweed.h"
#include "Sexy/Graphics.h"
#include "PvZ2/RoofHook.h"
#include "PvZ2/Board.h"
#include "Sexy/SexyApp.h"
#include "PvZ2/RoofStage.h"

typedef void (*plantSpikeweedRenderRigs)(PlantSpikeweed*, Sexy::Graphics*);
plantSpikeweedRenderRigs oPlantSpikeweedRenderRigs = NULL;

void hkPlantSpikeweedRenderRigs(PlantSpikeweed* plant, Sexy::Graphics* graphics) {
	if (plant->m_plantfoodSpikesActive) {
		int offsetX = 0x87;

		for (int i = 0; i < 9; ++i, offsetX += GRID_TILE_WIDTH) {
			if (plant->m_plantfoodSpikes[i].IsValid()) {
				float offsetY = GRID_TILE_HEIGHT * plant->m_plant->m_row;

				float mX = GRID_TILE_LEFT + GRID_TILE_WIDTH / 2 + GRID_TILE_WIDTH * i;
				offsetY -= GetSlopeOffset(mX, offsetY);

				auto sexyApp = Sexy::SexyApp::Get();
				float scaledX = sexyApp->Scale(offsetX);
				float scaledY = sexyApp->Scale(offsetY + 0x68);

				// LOGI("ScaledX = %f ScaledY = %f", scaledX, scaledY);

				graphics->mTransX += scaledX;
				graphics->mTransY += scaledY;

				auto animRig = plant->m_plantfoodSpikes[i].Get();
				animRig->Render(graphics);

				graphics->mTransX -= scaledX;
				graphics->mTransY -= scaledY;
			}
		}
	}

	hkPlantRenderRigs(plant->m_plant, graphics);
}

void initRoofPlantFixes() {
	FluffyHookFunction(0xAFCE3C, (void*)hkPlantSpikeweedRenderRigs, (void**)&oPlantSpikeweedRenderRigs);
}