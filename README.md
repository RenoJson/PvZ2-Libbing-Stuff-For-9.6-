# **PvZ2 Libbing Stuff For 9.6**

# Credits

* Blazey - original template
* Brekko/Jay - original repositories (also thanks for helping me on softcode the Bull)
* Snowie and Vi - helping Blazey making original template

# What is this?

An expanded library for PvZ2 (9.6.1), based on Brekko's ExampleMod. I want it to be fully open sourced and public. My main goal is try to softcode any zombies via reflection system, which adds new properties to packages. It's currently in development, and no ARM64 version is available at the moment. 

# Current Features
* IntegerID backport! - Plant/Zombie types now support id mapping, which will help modders who dont want to constantly update the apk.
* Power Lily softcode - Power Lily gained new properties regarding her collectable
* Arcade Pusher GI softcode
* Mummy Memory from the original Example Mod
* Beta softcode for Bull and Chair Racer (unfinished due to broken hypno checking)

# Future Plans
- Fixing hypno stuff
- Create new projectile class based from SquidProjectile
- Hooking General Base class to be able to view the health bar
- Port my stuff to 64 bit once there are 64 bit template

# Build Instructions
Building this repo is the same as building Blazey example mod update. The instructions regarding that [you can find here](https://github.com/BlazeyLol/PVZ2ExampleMod) or [in this old doc](https://docs.google.com/document/d/1egwNqNJjw61MlLYMgzUewdTkztpB-6VruHrSez90gcA/edit?usp=sharing).
This library requires some packages modifications in order to work. You can find example Plant/Zombie types jsons in the Packages directory.
If you want to utilize Power Lily's custom fields, add these to her props:
- CollectableType (string)
- CollectableCount (integer)
- CollectableOffsetY (float)

Arcade's new property is `GridItemType` (string). One important note: DO NOT use a grid item which class isn't `GridItemEightiesArcadeCabinet`, otherwise zombie won't be able to push it.

# Bull's new property:
- RiderType (string)
- RiderLayerToHide (string list)
- LandOffsetX (float)
- NOTE: The RiderType must use ZombieImp class with BullRiderProps. You can using RiderLayerToHide and LandOffSetX or not as they will use the original one if you don't use them. Update 1/10/2025: This softcode only work on Bull class and props, for the veteran, only using "RiderType": "west_bullrider" to make it work, any softcode for bull veteran could make the game crash.
# ZcorpRacer's (Chair Racer) new property:
- RacerType (string)
- NOTE: Beside the RacerType must use ZcorpRacerZombie class, both the chair and racer must have zcorp_racer_chair and zcorp_racer in their typename or the chair is completely frozen sob, also don't use replicate in custom chair racer typename or the game just crash if you enter a level that having them (don't ask me why).
