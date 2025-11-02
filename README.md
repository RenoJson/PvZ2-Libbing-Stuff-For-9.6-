# **PvZ2 Libbing Stuff For 9.6**

# Credits

* Blazey - original template
* Brekko/Jay - original repositories (also thanks for helping me on softcode the Bull and fixing them)
* Snowie and Vi - helping Blazey making original template

# What is this?

An expanded library for PvZ2 (9.6.1), based on Brekko's ExampleMod. I want it to be fully open sourced and public. My main goal is try to softcode any zombies via reflection system, which adds new properties to packages. It's currently in development, and no ARM64 version is available at the moment. 

# Current Features
* IntegerID backport! - Plant/Zombie types now support id mapping, which will help modders who dont want to constantly update the apk.
* Arcade Pusher GI softcode
* Mummy Memory from the original Example Mod
* Stable softcode for Bull and Chair Racer, now it's shouldn't have any bug anymore for now. Also, now you can use the bull softcode for veteran bull too. Check the sample.json file in packages to learn how to use them.

# Future Plans
- Port my stuff to 64 bit once there are 64 bit template

# Build Instructions
Building this repo is the same as building Blazey example mod update. The instructions regarding that [you can find here](https://github.com/BlazeyLol/PVZ2ExampleMod) and [here](https://github.com/BlazeyLol/PVZ2ExpansionMod) or [in this old doc](https://docs.google.com/document/d/1egwNqNJjw61MlLYMgzUewdTkztpB-6VruHrSez90gcA/edit?usp=sharing).
This library requires some packages modifications in order to work. You can find example Plant/Zombie types jsons in the Packages directory.

Arcade's new property is `GridItemType` (string). One important note: DO NOT use a grid item which class isn't `GridItemEightiesArcadeCabinet`, otherwise zombie won't be able to push it.

# Bull's and Veteran Bull's new property:
- RiderType (string) (using this for bull props) 
- VetRiderType (string) (using this for veteran bull props)
- RiderLayerToHide (string list) (this can use for both props)
- LaunchDistance (float) (this can use for both props)
- NOTE: The RiderType and VetRiderType must use ZombieImp class with BullRiderProps. If you want using any vanilla props of them, just not use them as the function will using the hardcode value instead.

# ZcorpRacer's (Chair Racer) new property:
- RacerType (string)
- NOTE: Beside the RacerType must use ZcorpRacerZombie class. If you want using any vanilla props of them, just not use them as the function will using the hardcode value instead.
