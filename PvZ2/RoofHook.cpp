#include "pch.h"
#include "PvZ2/RoofHook.h"
#include "PvZ2/Board.h"
#include "memUtils.h"
#include "PacketCursor.h"
#include "ZombieParticle.h"
#include "RoofModule.h"
#include "RoofModuleProps.h"
#include "Sexy/Graphics.h"
#include "Sexy/PopAnim.h"
#include "TimeMgr.h"
#include "PlantPropertySheet.h"
#include "RoofPlantFixes.h"
#include "RoofStage.h"
#include "RoofStageProps.h"
#include "Sexy/SexyApp.h"
#include "PvZ2/Effect_PopAnim.h"

float getKillHeight(float mX, float mY) {
	auto roofStage = RoofStage::Get();
	if (roofStage) {
		return roofStage->GetKillDepth(mX, mY);
	}
	else {
		return -MAXFLOAT;
	}
}

typedef void (*plantCursorGetGridCoords)(int*, int);
plantCursorGetGridCoords oPlantCursorGetGridCoords = NULL;

void hkPlantCursorGetGridCoords(int* res, int cursor) {
	int mX = *(int*)(cursor + 4);
	int mY = *(int*)(cursor + 8);
	*(int*)(cursor + 8) += GetSlopeOffset(mX, mY);

	oPlantCursorGetGridCoords(res, cursor);

	*(int*)(cursor + 8) = mY;
}

typedef int (*convertCursorToGridX)(int);
convertCursorToGridX oConvertCursorToGridX = NULL;

// todo: this is stupidly hacky but its the cleanest solution since getGridX is always called before getGridY

int savedX;
int hkConvertCursorToGridX(int x) {
	savedX = x;
	return oConvertCursorToGridX(x);
}

typedef int (*convertCursorToGridY)(int);
convertCursorToGridY oConvertCursorToGridY = NULL;

int hkConvertCursorToGridY(int y) {
	y += GetSlopeOffset(savedX, y);

	return oConvertCursorToGridY(y);
}

typedef void (*drawBoardCursorHighlights)(Board*, int);
drawBoardCursorHighlights oDrawBoardCursorHighlights = NULL;

void hkDrawBoardCursorHighlights(Board* board, int graphicsState) {
	for (void* cursorPtr : board->m_cursors) {
		int cursor = (int)cursorPtr;
		if (!*(bool*)(cursor + 0x1C)) {
			int mY = *(int*)(cursor + 8);
			int mX = *(int*)(cursor + 4);

			typedef bool (*isType)(int, Sexy::RtClass*);
			isType pIsType = *(isType*)(*(uint*)cursor + 0x10);

			typedef Sexy::RtClass* (*getType)();
			auto packetCursorType = PacketCursor::getRtClass();
			if (pIsType(cursor, packetCursorType)) {
				PacketCursor* cursorCasted = (PacketCursor*)cursor;

				typedef bool (*sub3f7acc)(PacketCursor*); // some kind of boundary checking function? 
				if (((sub3f7acc)getActualOffset(0x3F7ACC))(cursorCasted) || !cursorCasted->CanPlantOnCursor()) {
					continue;
				}

				typedef void (*getCursorCoords)(int*, int);
				getCursorCoords pFunc = *(getCursorCoords*)(*(uint*)cursor + 0x34);
				int res[2];
				pFunc(res, cursor);

				typedef void (*drawHighlights)(Board*, int, int, int); 
				((drawHighlights)getActualOffset(0x72C160))(board, graphicsState, res[0], res[1]);
				continue;
			}

			auto shovelCursorType = ((getType)getActualOffset(0x105E1C0))(); 
			auto pfCursorType = ((getType)getActualOffset(0x89C300))(); 

			typedef bool (*sub3f7acc)(int); // is in bounds function 9.4
			if ((pIsType(cursor, shovelCursorType) || pIsType(cursor, pfCursorType)) &&
				((sub3f7acc)getActualOffset(0x3F7ACC))(cursor)) {
				continue;
			}

			int gridX = hkConvertCursorToGridX(mX);
			int gridY = hkConvertCursorToGridY(mY);

			if (gridX < 0 || gridY < 0) {
				return;
			}

			typedef void (*drawHighlights)(Board*, int, int, int);
			((drawHighlights)getActualOffset(0x72C160))(board, graphicsState, gridX, gridY); 
		}
	}
}

typedef float (*realObjectGetSinkDist)(RealObject*, SexyVector3*);
realObjectGetSinkDist oRealObjGetSinkDist = NULL;

float hkRealObjectGetSinkDist(RealObject* a1, SexyVector3* a2) { // this should constraint board entities
	float ans = oRealObjGetSinkDist(a1, a2);
	// now add the roof sinking
	float offset = GetSlopeOffset(a2->mX, a2->mY);

	/*
	typedef Sexy::RtClass* (*GridItemAnimGetClass)();
	auto animClass = ((GridItemAnimGetClass)getActualOffset(0x411984))();
	if (a1->IsType(animClass)) { // rendering of these do not scale mZ up. Should probably be fixed at the root
		// this brings another problem: death anim now renders at a weird offset
		offset = SexyApp::Get()->Scale(offset);
	}
	*/

	ans += offset;

	return ans;
}

typedef float (*projUpdatePosition)(Projectile*, SexyVector3*);
projUpdatePosition oProjUpdatePosition = NULL;

float hkProjUpdatePosition(Projectile* proj, SexyVector3* pos) {
	LOGI("current position = %f %f %f", proj->m_position.mX, proj->m_position.mY, proj->m_position.mZ);
	/*if (pos->mX < GRID_TILE_LEFT + 5 * GRID_TILE_WIDTH) {
		pos->mZ -= ((GRID_TILE_LEFT + 5 * GRID_TILE_WIDTH) - pos->mX) / 4;
	}*/
	LOG_RETURN_ADDRESS(0);
	return oProjUpdatePosition(proj, pos);
}

typedef Projectile* (*plantCreateProj)(Plant*);
plantCreateProj oPlantCreateProj = NULL;

Projectile* hkPlantCreateProj(Plant* plant) {
	Projectile* res = oPlantCreateProj(plant);
	if (res->m_position.mX < GRID_TILE_LEFT + 5 * GRID_TILE_WIDTH) {
		res->m_position.mZ -= ((GRID_TILE_LEFT + 5 * GRID_TILE_WIDTH) - res->m_position.mX) / 4;
	}
	return res;
}

typedef void (*plantRenderShadows)(Plant*, int);
plantRenderShadows oPlantRenderShadows = NULL;

// TODO: just edit plant and zombies' shadow code to be the same as projectiles
void hkPlantRenderShadows(Plant* plant, int graphicsState) {
	// TODO: take into account relocation code
	float ogY = plant->m_position.mY;
	plant->m_position.mY -= GetSlopeOffset(plant->m_position.mX, plant->m_position.mY);
	oPlantRenderShadows(plant, graphicsState);
	plant->m_position.mY = ogY;
}

typedef void (*zombieRenderShadows)(Zombie*, int);
zombieRenderShadows oZombieRenderShadows = NULL;

void hkZombieRenderShadows(Zombie* zombie, int graphicsState) {
	float ogY = zombie->m_position.mY;
	zombie->m_position.mY -= GetSlopeOffset(zombie->m_position.mX, zombie->m_position.mY);
	oZombieRenderShadows(zombie, graphicsState);
	zombie->m_position.mY = ogY;
}

typedef void (*projRenderShadows)(Projectile*, int);
projRenderShadows oProjRenderShadows = NULL;

void hkProjectileRenderShadows(Projectile* proj, int graphicsState) {
	float ogY = proj->m_position.mY;
	proj->m_position.mY -= GetSlopeOffset(proj->m_position.mX, proj->m_position.mY);
	oProjRenderShadows(proj, graphicsState);
	proj->m_position.mY = ogY;
}

typedef void (*projUpdateVelocity)(Projectile*, float);
projUpdateVelocity oProjUpdateVelocity = NULL;

void hkProjectileUpdateVelocity(Projectile* proj, float a2) { // a2 is timeStep
	oProjUpdateVelocity(proj, a2);

	auto roofModule = RoofModule::GetInstance();

	if (roofModule) {
		bool followsSlope = roofModule->IsProjectileFollowsSlope(proj);

		if (followsSlope) {
			proj->UpdateSinkDistance(false);
		}

		if (!roofModule->IsProjectileImmune(proj) && !followsSlope) {
			float killHeight = getKillHeight(proj->m_position.mX, proj->m_position.mY);
			int tolerance = roofModule->GetProjectileTolerance(proj);

			if (proj->m_position.mZ + tolerance < killHeight) {
				proj->PlayImpact(NULL);
				proj->Die();
			}
		}
	}
}

typedef Projectile* (*addProjectileByType)(Board*, float, float, float, Sexy::RtWeakPtr<ProjectilePropertySheet>*, BoardEntity*);
addProjectileByType oAddProjectileByType = NULL;

Projectile* hkAddProjectileByType(Board* board, float mX, float mY, float mZ, Sexy::RtWeakPtr<ProjectilePropertySheet>* type, BoardEntity* owner) {
	if (owner && owner->IsType(Plant::getRtClass())) {
		auto roofModule = RoofModule::GetInstance();
		if (roofModule) {
			if (!roofModule->IsProjectileSpawnIgnoresSlope(type)) {
				mZ += owner->m_position.mZ;
				LOGI("Projectile doesn't ignore slope, spawns at %f", mZ);
			}
		}
	}

	return oAddProjectileByType(board, mX, mY, mZ, type, owner);
}

typedef void (*setFromResource)(Effect_PopAnim*, int, int);
setFromResource oSetFromResource = NULL;

float SexyAppScale(int gSexyApp, float a2) {
	return *(float*)(gSexyApp + 0x698) * a2;
}

float SexyAppUnscale(int gSexyApp, float a2) {
	return a2 / *(float*)(gSexyApp + 0x698);
}

void hkSetFromResource(Effect_PopAnim* a1, int a2, int objClass) {
	int offsets[2];
	auto roofModule = RoofModule::GetInstance();
	if (roofModule && roofModule->IsEffectSloped(offsets, (Sexy::RtWeakPtr<void>*)(a2 + 8))) {
		float mX = SexyAppUnscale(getSexyApp(), a1->screenSpaceOrigin.mX);
		float mY = SexyAppUnscale(getSexyApp(), a1->screenSpaceOrigin.mY);
		float slopeHeight = GetSlopeOffset(mX + offsets[0], mY + offsets[1]);
		a1->screenSpaceOrigin.mY -= SexyAppScale(getSexyApp(), slopeHeight);
	}

	oSetFromResource(a1, a2, objClass);
}

typedef void (*setPositionAndLayer)(Effect_PopAnim*, SexyVector3*, int);
setPositionAndLayer oSetPositionAndLayer = NULL;

void hkSetPositionAndLayer(Effect_PopAnim* a1, SexyVector3* a2, int layer) {
	if (a1->animRig.IsValid()) {
		PopAnimRig* rig = a1->animRig.Get();
		Sexy::RtWeakPtr<void> rigPamRtId;
		rigPamRtId.FromOther(&rig->m_popAnim->pamRtId);

		int offsets[2];
		auto roofModule = RoofModule::GetInstance();
		if (roofModule && roofModule->IsEffectSloped(offsets, &rigPamRtId)) {
			float slopeHeight = GetSlopeOffset(a2->mX + offsets[0], a2->mY + offsets[1]);
			a2->mZ += slopeHeight;
		}
	}

	oSetPositionAndLayer(a1, a2, layer);
}

typedef void (*plantRenderRigs)(Plant*, Sexy::Graphics*);
plantRenderRigs oPlantRenderRigs = NULL;

void hkPlantRenderRigs(Plant* plant, Sexy::Graphics* graphics) {
	if ((plant->m_realObjectFlags & 4) == 0) {
		graphics->UnkFunc();

		plant->UpdateGroundClipRect(graphics);

		float v8 = 0.0; // I think this is rotation related
		if (plant->m_relocating) {
			typedef float (*subE90C8C)(Plant*);
			v8 = ((subE90C8C)getActualOffset(0xE90C8C))(plant); 

			float v6 = SexyAppScale(getSexyApp(), plant->m_relocateToX + plant->m_relocationOffsetFromDestination.mX);
			graphics->mTransX += graphics->mScaleOrigX + v6;

			float v9 = SexyAppScale(getSexyApp(), plant->m_relocateToY + plant->m_relocationOffsetFromDestination.mY);
			graphics->mTransY += graphics->mScaleOrigY + v9;
		}
		else {
			float mXScaled = SexyAppScale(getSexyApp(), plant->m_position.mX);
			graphics->mTransX += graphics->mScaleOrigX + (mXScaled - graphics->mScaleOrigX) * graphics->mScaleX;

			float mYScaled = SexyAppScale(getSexyApp(), plant->m_position.mY);
			graphics->mTransY += graphics->mScaleOrigY + (mYScaled - graphics->mScaleOrigY) * graphics->mScaleY;
		}

		float ogTransX = graphics->mTransX;
		float ogTransY = graphics->mTransY;

		PopAnimRig* plantfoodShine = (PopAnimRig*)plant->m_plantFoodShine.Get();
		if (plantfoodShine->m_state) {
			float v21 = SexyAppScale(getSexyApp(), -68.0);
			float v23 = SexyAppScale(getSexyApp(), -160.0);

			// Added code to render plantfoodShine at Z offset
			float v40 = CallVirtualFunc<float, void*>(plant->m_plantFramework, 13, plant->m_plantFramework);
			graphics->mTransY += SexyAppScale(getSexyApp(), v40 - plant->m_position.mZ) * graphics->mScaleY;
			// end of added code

			Sexy::Transform2D transMatrix;
			transMatrix.mat[0][0] *= graphics->mScaleX * 1.5;
			transMatrix.mat[0][1] *= graphics->mScaleX * 1.5;
			transMatrix.mat[0][2] = v21 * graphics->mScaleX * 1.5 + graphics->mScaleX * transMatrix.mat[0][2];
			transMatrix.mat[1][0] *= graphics->mScaleY * 1.5;
			transMatrix.mat[1][1] *= graphics->mScaleY * 1.5;
			transMatrix.mat[1][2] = graphics->mScaleY * 1.5 * transMatrix.mat[1][2] + v23 * graphics->mScaleY * 1.5;
			plantfoodShine->TransformAndRender(graphics, transMatrix);

			graphics->mTransX = ogTransX;
			graphics->mTransY = ogTransY;
		}

		PopAnimRig* animRig = (PopAnimRig*)plant->m_animRig.Get();
		if (animRig) {
			Sexy::Color color = animRig->GetColor();
			if (plant->m_fadeInEndTime < MAXFLOAT) {
				float fadeInStartTime = plant->m_fadeInEndTime - plant->m_fadeInDuration;
				float opacity = (TimeMgr::GetInstance()->m_curTime - fadeInStartTime) / plant->m_fadeInDuration;
				if (opacity < 0) {
					opacity = 0;
				}
				else {
					opacity = std::min(opacity, 1.0f);

					/*
					typedef void (*interpolateOpacity)(float opacity, int type);
					opacity = ((interpolateOpacity)getActualOffset(0x46CDFC))(opacity, 2);
					*/
					opacity *= opacity;
				}

				color.alpha = opacity * 255;
				animRig->SetColor(color);
			}

			float v40 = CallVirtualFunc<float, void*>(plant->m_plantFramework, 13, plant->m_plantFramework);
			graphics->mTransY += SexyAppScale(getSexyApp(), v40 - plant->m_position.mZ) * graphics->mScaleY;

			float sproutScale = 1.0f;

			if (plant->m_sproutScalingEndTime < MAXFLOAT) {
				float sproutScalingStartTime = plant->m_sproutScalingEndTime - plant->m_sproutScalingDuration;
				float scale = (TimeMgr::GetInstance()->m_curTime - sproutScalingStartTime) / plant->m_sproutScalingDuration;
				scale = std::max(scale, 0.0f);
				scale = std::min(scale, 1.0f);
				scale *= scale;
				if (TimeMgr::GetInstance()->m_curTime >= plant->m_sproutScalingEndTime) {
					plant->m_sproutScalingEndTime = MAXFLOAT;
				}
				sproutScale = scale;
			}

			Sexy::Transform2D transMatrix;
			if (v8 != 0.0f) {
				int v80[4];
				memset(v80, 0, sizeof(v80));
				// LOGI("before virtual func call v8 = %f", v8);
				// LOGI("anim rig = %u anim rig func 16 = %u vtbl = %u", (uint)animRig, getOriginalOffset((uint) GetVirtualFunc(animRig, 16)), getOriginalOffset(*(uint*)animRig));
				CallVirtualFunc<void, void*, int*>(animRig, 16, animRig, v80);
				// LOGI("After call");
				float v52 = v80[0] + v80[2] * 0.5;
				float v53 = v80[1] + v80[3] * 0.5;
				transMatrix.mat[0][2] -= v52;
				transMatrix.mat[1][2] -= v53;
				typedef void (*rotate)(Sexy::Transform2D*, float);
				((rotate)getActualOffset(0x34AED4))(&transMatrix, v8); 
				transMatrix.mat[0][2] += v52;
				transMatrix.mat[1][2] += v53;
			}

			float v54 = sproutScale * plant->m_baseScale * graphics->mScaleX;
			if (plant->m_mirror) {
				transMatrix.mat[0][0] *= -v54;
				transMatrix.mat[0][1] *= -v54;
				transMatrix.mat[0][2] *= -v54;
				transMatrix.mat[1][0] *= v54;
				transMatrix.mat[1][1] *= v54;
				transMatrix.mat[1][2] *= v54;

				auto props = plant->GetProps<PlantPropertySheet>();
				float scaledCenterX = SexyAppScale(getSexyApp(), props->ArtCenter.mX * -v54);
				float scaledCenterY = SexyAppScale(getSexyApp(), props->ArtCenter.mY * v54);
				transMatrix.mat[0][2] -= scaledCenterX;
				transMatrix.mat[1][2] -= scaledCenterY;
			}
			else {
				for (int i = 0; i < 2; ++i) {
					for (int j = 0; j < 3; ++j) {
						transMatrix.mat[i][j] *= v54;
					}
				}
				auto props = plant->GetProps<PlantPropertySheet>();
				float scaledCenterX = SexyAppScale(getSexyApp(), props->ArtCenter.mX * v54);
				float scaledCenterY = SexyAppScale(getSexyApp(), props->ArtCenter.mY * v54);
				transMatrix.mat[0][2] -= scaledCenterX;
				transMatrix.mat[1][2] -= scaledCenterY;
			}

			animRig->TransformAndRender(graphics, transMatrix);
		}

		graphics->UnkFunc2();
	}
}

typedef void (*gridItemAnimRender)(BoardEntity*, Sexy::Graphics*);
gridItemAnimRender oGridItemAnimRender = NULL;

void hkGridItemAnimRender(BoardEntity* entity, Sexy::Graphics* graphics) { // mZ does not get scaled in the code for whatever reason
	float ogmZ = entity->m_position.mZ;
	entity->m_position.mZ = Sexy::SexyApp::Get()->Scale(entity->m_position.mZ);
	oGridItemAnimRender(entity, graphics);
	entity->m_position.mZ = ogmZ;
}

typedef void (*fun302)(int, Zombie*, int, char);
fun302 oFun302 = NULL;

void hkFun302(int a1, Zombie* a2, int a3, char a4) {
	if (a2->IsType(Zombie::getRtClass())) {
		return;
	}
	oFun302(a1, a2, a3, a4);
}

typedef void (*fun303)(Zombie*, Sexy::Graphics*);
fun303 oFun303 = NULL;

void hkFun303(Zombie* a1, Sexy::Graphics* a2) {
	// a1->m_realObjectFlags &= ~1u;

	oFun303(a1, a2);

	a1->m_realObjectFlags |= 1;
}

typedef void (*zombieClipGroundEffect)(Rect*, Zombie*, char*);
zombieClipGroundEffect oZombieClipGroundEffect = NULL;

void hkZombieClipGroundEffect(Rect* a1, Zombie* a2, char* a3) {
	float ogY = a2->m_position.mY;
	a2->m_position.mY -= GetSlopeOffset(a2->m_position.mX, a2->m_position.mY);
	oZombieClipGroundEffect(a1, a2, a3);
	a2->m_position.mY = ogY;
}
void initRoofHooks() {

	FluffyHookFunction(0xCFF444, (void*)hkPlantCursorGetGridCoords, (void**)&oPlantCursorGetGridCoords); //done
	FluffyHookFunction(0x63440C, (void*)hkConvertCursorToGridX, (void**)&oConvertCursorToGridX); //done
	FluffyHookFunction(0x634428, (void*)hkConvertCursorToGridY, (void**)&oConvertCursorToGridY);//done

	FluffyHookFunction(0x72B1C0, (void*)hkDrawBoardCursorHighlights, (void**)&oDrawBoardCursorHighlights);//done

	// FluffyHookFunction(0xE8B690, (void*)hkFun292, (void**)&oFun292, "fun 292");
	FluffyHookFunction(0xCEB8D8, (void*)hkRealObjectGetSinkDist, (void**)&oRealObjGetSinkDist); //done

	// now to fix projectile spawns. Override getProjectileOffset
	FluffyHookFunction(0x29D66C, (void*)hkProjUpdatePosition, (void**)&oProjUpdatePosition); //done
	FluffyHookFunction(0xE952D8, (void*)hkPlantCreateProj, (void**)&oPlantCreateProj); //done
	// zombie created projectiles are probably fine since they needed to support tides already
	// still need to handle PF projectiles

	FluffyHookFunction(0xE93C48, (void*)hkPlantRenderShadows, (void**)&oPlantRenderShadows); //done
	FluffyHookFunction(0x8B6098, (void*)hkZombieRenderShadows, (void**)&oZombieRenderShadows); //done
	FluffyHookFunction(0x90BA38, (void*)hkProjectileRenderShadows, (void**)&oProjRenderShadows); //done

	FluffyHookFunction(0x90965C, (void*)hkProjectileUpdateVelocity, (void**)&oProjUpdateVelocity); //done

	// TODO: we got some more shit like zombie particles and fallen sun (from shovel) to deal with

	// TODO: Uhh, recreate the ActivatePlantfood and CreateProjectile functions to pass the Plant itself into offsets. Orrr
	//	just override them and the GetProjectileOffset function to maintain the Plant

	// TODO: DrawPlantingHighlights is called from another source: vasebreaker powerup stuff. Idc rn

	FluffyHookFunction(0x720A2C, (void*)hkAddProjectileByType, (void**)&oAddProjectileByType); //done
	// FluffyHookFunction(0x1246A14, (void*)hkFun301, (void**)&oFun301, "fun 301");
	FluffyHookFunction(0xE56188, (void*)hkSetFromResource, (void**)&oSetFromResource); //done
	FluffyHookFunction(0x735B60, (void*)hkSetPositionAndLayer, (void**)&oSetPositionAndLayer); //done

	// FluffyHookFunction(0xC90A14, (void*)hkFun300, (void**)&oFun300, "fun 300");
	FluffyHookFunction(0xE94088, (void*)hkPlantRenderRigs, (void**)&oPlantRenderRigs); //done

	FluffyHookFunction(0x405B9C, (void*)hkGridItemAnimRender, (void**)&oGridItemAnimRender); //done

	// FluffyHookFunction(0xFE4E4C, (void*)hkFun302, (void**)&oFun302, "fun 302");
	// FluffyHookFunction(0x8BE450, (void*)hkFun303, (void**)&oFun303, "fun 303");
	FluffyHookFunction(0xCEB710, (void*)hkZombieClipGroundEffect, (void**)&oZombieClipGroundEffect); //done

	RoofModule::modInit();
	RoofModuleProps::modInit();

	RoofStage::modInit();
	RoofStageProps::modInit();

	initRoofPlantFixes();
}