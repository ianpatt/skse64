Scriptname DefaultObjectManager extends Form Hidden

; Returns the default form for this key e.g. 'GOLD'
Form Function GetForm(string key) native

; Sets the default form for the particular key
Function SetForm(string key, Form newForm) native

; Valid Keys
; WWSP - Werewolf Spell
; SALT - Sitting Angle Limit
; APSH - Allow Player Shout
; GOLD - Gold
; LKPK - Lockpick
; SKLK - SkeletonKey
; PFAC - Player Faction
; GFAC - Guard Faction
; DFMS - Default Music
; BTMS - Battle Music
; DTMS - Death Music
; SCMS - Success Music
; LUMS - Level Up Music
; DCMS - Dungeon Cleared Music
; PVMA - Player Voice (Male)
; PVMC - Player Voice (Male Child)
; PVFA - Player Voice (Female)
; PVFC - Player Voice (Female Child)
; EPDF - Eat Package Default Food
; LHEQ - LeftHand Equip
; RHEQ - RightHand Equip
; EHEQ - EitherHand Equip
; VOEQ - Voice Equip
; POEQ - Potion Equip
; EACA - Every Actor Ability
; CACA - Commanded Actor Ability
; DEIS - Drug Wears Off Image Space
; DFTS - Footstep Set
; DLMT - Landscape Material
; DLZM - Dragon Land Zone Marker
; DCZM - Dragon Crash Zone Marker
; CSTY - Combat Style
; PLST - Default Pack List
; PWFD - Wait-For-Dialogue Package
; LRTB - LocRefType Boss
; VLOC - Virtual Location
; PLOC - PersistAll Location
; INVP - Inventory Player
; PTNP - Pathing Test NPC
; FPCS - Favor Cost Small
; FPCM - Favor Cost Medium
; FPCL - Favor Cost Large
; FGPD - Favor Gifts Per Day
; AASW - Action Swim State Change
; AALK - Action Look
; AALA - Action LeftAttack
; AALD - Action LeftReady
; AALR - Action LeftRelease
; AALI - Action LeftInterrupt
; AARA - Action RightAttack
; AARD - Action RightReady
; AARR - Action RightRelease
; AARI - Action RightInterrupt
; AADA - Action DualAttack
; AADL - Action DualRelease
; AAAC - Action Activate
; AAJP - Action Jump
; AAFA - Action Fall
; AALN - Action Land
; AASN - Action Sneak
; AAVC - Action Voice
; AAVD - Action VoiceReady
; AAVR - Action VoiceRelease
; AAVI - Action VoiceInterrupt
; AAID - Action Idle
; AAST - Action Sprint Start
; AASP - Action Sprint Stop
; AADR - Action Draw
; AASH - Action Sheath
; ALPA - Action Left Power Attack
; AAPA - Action Right Power Attack
; ADPA - Action Dual Power Attack
; AAS1 - Action Stagger Start
; AABH - Action Block Hit
; AABA - Action Block Anticipate
; AARC - Action Recoil
; AAR2 - Action Large Recoil
; AAB1 - Action Bleedout Start
; AAB2 - Action Bleedout Stop
; AAIS - Action Idle Stop
; AAWH - Action Ward Hit
; AAFQ - Action Force Equip
; AASC - Action Shield Change
; AAPS - Action Path Start
; AAPE - Action Path End
; AALM - Action Large Movement Delta
; AAF1 - Action Fly Start
; AAF2 - Action Fly Stop
; AAH1 - Action Hover Start
; AAH2 - Action Hover Stop
; AABI - Action Bumped Into
; AASS - Action Summoned Start
; ATKI - Action Talking Idle
; ALTI - Action Listen Idle
; AADE - Action Death
; AADW - Action Death Wait
; AIDW - Action Idle Warn
; AMST - Action Move Start
; AMSP - Action Move Stop
; ATRI - Action Turn Right
; ATLE - Action Turn Left
; ATSP - Action Turn Stop
; AMFD - Action Move Forward
; AMBK - Action Move Backward
; AMLT - Action Move Left
; AMRT - Action Move Right
; ARAG - Action Reset Animation Graph
; AKDN - Action Knockdown
; AAGU - Action Get Up
; ASID - Action Idle Stop Instant
; ARGI - Action Ragdoll Instant
; AWWS - Action Waterwalk Start
; AREL - Action Reload
; PUSG - Pickup Sound Generic
; PDSG - Putdown Sound Generic
; PUSW - Pickup Sound Weapon
; PDSW - Putdown Sound Weapon
; PUSA - Pickup Sound Armor
; PDSA - Putdown Sound Armor
; PUSB - Pickup Sound Book
; PDSB - Putdown Sound Book
; PUSI - Pickup Sound Ingredient
; PDSI - Putdown Sound Ingredient
; HVSS - Harvest Sound
; HVFS - Harvest Failed Sound
; WBSN - Ward Break Sound
; WASN - Ward Absorb Sound
; WDSN - Ward Deflect Sound
; MFSN - Magic Fail Sound
; SFSN - Shout Fail Sound
; HFSD - Heartbeat Sound Fast
; HSSD - Heartbeat Sound Slow
; IMLH - Imagespace: Low Health
; SCSD - Soul Captured Sound
; NASD - No-Activation Sound
; MMSD - Map Menu Looping Sound
; DDSC - Dialogue Voice Category
; NDSC - Non-Dialogue Voice Category
; SFDC - SFX To Fade In Dialogue Category
; PDMC - Pause During Menu Category (Fade)
; PIMC - Pause During Menu Category (Immediate)
; PDLC - Pause During Loading Menu Category
; MDSC - Music Sound Category
; SMSC - Stats Mute Category
; SSSC - Stats Music
; MTSC - Master Sound Category
; TSSC - Time Sensitive Sound Category
; DOP2 - Dialogue Output Model (3D)
; DOP3 - Dialogue Output Model (2D)
; POPM - Player's Output Model (1st Person)
; P3OM - Player's Output Model (3rd Person)
; IOPM - Interface Output Model
; RVBT - Reverb Type
; UWLS - Underwater Loop Sound
; URVT - Underwater Reverb Type
; HRSK - Keyword - Horse
; UNDK - Keyword - Undead
; NPCK - Keyword - NPC
; KWBR - Keyword - BeastRace
; KWDM - Keyword - DummyObject
; KWGE - Keyword - UseGeometryEmitter
; KWMS - Keyword - MustStop
; KWUA - Keyword - UpdateDuringArchery
; KWOT - Keyword - Skip Outfit Items
; FTHD - Male Face Texture Set: Head
; FTMO - Male Face Texture Set: Mouth
; FTEL - Male Face Texture Set: Eyes
; FTHF - Female Face Texture Set: Head
; FTMF - Female Face Texture Set: Mouth
; FTRF - Female Face Texture Set: Eyes
; IMID - ImageSpaceModifier for inventory menu.
; PTEM - Package template
; MMCL - Main Menu Cell
; DMWL - Default MovementType: Walk
; DMRN - Default MovementType: Run
; DMSW - Default MovementType: Swim
; DMFL - Default MovementType: Fly
; DMSN - Default MovementType: Sneak
; DMSP - Default MovementType: Sprint
; SPFK - Keyword - Special Furniture
; FFFP - Keyword - Furniture Forces 1st Person
; FFTP - Keyword - Furniture Forces 3rd Person
; AFNP - Keyword - Activator Furniture No Player
; TKGS - Telekinesis Grab Sound
; TKTS - Telekinesis Throw Sound
; WMWE - World Map Weather
; HMPC - Help Manual PC
; HMXB - Help Manual XBox
; TKAM - Keyword - Type Ammo
; TKAR - Keyword - Type Armor
; TKBK - Keyword - Type Book
; TKIG - Keyword - Type Ingredient
; TKKY - Keyword - Type Key
; TKMS - Keyword - Type Misc
; TKSG - Keyword - Type SoulGem
; TKWP - Keyword - Type Weapon
; TKPT - Keyword - Type Potion
; BENW - Base Weapon Enchantment
; BENA - Base Armor Enchantment
; BAPO - Base Potion
; BAPS - Base Poison
; DRAK - Keyword - Dragon
; MVBL - Keyword - Movable
; ABSE - Art Object - Absorb Effect
; WEML - Weapon Material List
; ARTL - Armor Material List
; DIEN - Keyword - Disallow Enchanting
; FTML - Favor travel marker location
; LKHO - Keyword - Hold Location
; CWOK - Keyword - Civil War Owner
; CWNE - Keyword - Civil War Neutral
; LRSO - LocRefType - Civil War Soldier
; KWDO - Keyword - ClearableLocation
; LRRD - LocRefType - Resource Destructible
; HCLL - FormList - Hair Color List
; CMPX - Complex Scene Object
; RUSG - Keyword - Reusable SoulGem
; ANML - Keyword - Animal
; DAED - Keyword - Daedra
; BEEP - Keyword - Robot
; NRNT - Keyword - Nirnroot
; FTGF - Fighters' Guild Faction
; MGGF - Mages' Guild Faction
; TVGF - Thieves' Guild Faction
; DBHF - Dark Brotherhood Faction
; JRLF - Jarl Faction
; AWWW - Bunny Faction
; PIVV - Player Is Vampire Variable
; PIWV - Player Is Werewolf Variable
; NMRD - Road Marker
; SAT1 - Keyword: Scale Actor To 1.0
; VAMP - Keyword: Vampire
; FORG - Keyword: Forge
; COOK - Keyword: Cooking Pot
; SMLT - Keyword: Smelter
; TANN - Keyword: Tanning Rack
; HBLK - Help - Basic Lockpicking (PC)
; HBLX - Help - Basic Lockpicking (Console)
; HBFG - Help - Basic Forging
; HBCO - Help - Basic Cooking
; HBML - Help - Basic Smelting
; HBTA - Help - Basic Tanning
; HBOC - Help - Basic Object Creation
; HBEC - Help - Basic Enchanting
; HBSM - Help - Basic Smithing Weapon
; HBSA - Help - Basic Smithing Armor
; HBAL - Help - Basic Alchemy
; HBBR - Help - Barter
; HBLU - Help - Leveling up
; HBSK - Help - Skills Menu
; HBMM - Help - Map Menu
; HBJL - Help - Journal
; HBLH - Help - Low Health
; HBLM - Help - Low Magicka
; HBLS - Help - Low Stamina
; HBHJ - Help - Jail
; HBFT - Help - Teamate Favor
; HBWC - Help - Weapon Charge
; HBFS - Help - Favorites
; KHFL - Kinect Help FormList
; HBFM - Help - Flying Mount
; HBTL - Help - Target Lock
; HBAT - Help - Attack Target
; LSIS - Imagespace: Load screen
; WMDA - Keyword - Weapon Material Daedric
; WMDR - Keyword - Weapon Material Draugr
; WMDH - Keyword - Weapon Material DraugrHoned
; WMDW - Keyword - Weapon Material Dwarven
; WMEB - Keyword - Weapon Material Ebony
; WMEL - Keyword - Weapon Material Elven
; WMFA - Keyword - Weapon Material Falmer
; WMFH - Keyword - Weapon Material FalmerHoned
; WMGL - Keyword - Weapon Material Glass
; WMIM - Keyword - Weapon Material Imperial
; WMIR - Keyword - Weapon Material Iron
; WMOR - Keyword - Weapon Material Orcish
; WMST - Keyword - Weapon Material Steel
; WMWO - Keyword - Weapon Material Wood
; WTBA - Keyword - WeaponTypeBoundArrow
; AODA - Keyword - Armor Material Daedric
; AODP - Keyword - Armor Material Dragonplate
; AODS - Keyword - Armor Material Dragonscale
; AODB - Keyword - Armor Material Dragonbone
; AODW - Keyword - Armor Material Dwarven
; AOEB - Keyword - Armor Material Ebony
; AOEL - Keyword - Armor Material Elven
; AOES - Keyword - Armor Material ElvenSplinted
; AOFL - Keyword - Armor Material FullLeather
; AOGL - Keyword - Armor Material Glass
; AOHI - Keyword - Armor Material Hide
; AOIM - Keyword - Armor Material Imperial
; AOIH - Keyword - Armor Material ImperialHeavy
; AOIR - Keyword - Armor Material ImperialReinforced
; AOFE - Keyword - Armor Material Iron
; AOIB - Keyword - Armor Material IronBanded
; AOOR - Keyword - Armor Material Orcish
; AOSC - Keyword - Armor Material Scaled
; AOST - Keyword - Armor Material Steel
; AOSP - Keyword - Armor Material SteelPlate
; AOSK - Keyword - Armor Material Stormcloak
; AOSD - Keyword - Armor Material Studded
; GCK1 - Keyword - Generic Craftable Keyword 01
; GCK2 - Keyword - Generic Craftable Keyword 02
; GCK3 - Keyword - Generic Craftable Keyword 03
; GCK4 - Keyword - Generic Craftable Keyword 04
; GCK5 - Keyword - Generic Craftable Keyword 05
; GCK6 - Keyword - Generic Craftable Keyword 06
; GCK7 - Keyword - Generic Craftable Keyword 07
; GCK8 - Keyword - Generic Craftable Keyword 08
; GCK9 - Keyword - Generic Craftable Keyword 09
; GCKX - Keyword - Generic Craftable Keyword 10
; JWLR - Keyword - Jewelry
; KWCU - Keyword - Cuirass
; LMHP - Local Map Hide Plane
; SLDM - Snow LOD Material
; SLHD - Snow LOD Material (HD)
; ALDM - Ash LOD Material
; ALHD - Ash LOD Material (HD)
; DGFL - DialogueFollower Quest
; PTFR - PotentialFollower Faction
; AVWP - Werewolf Available Perks
; AVVP - Vampire Available Perks
; RIWR - Werewolf Race
; RIVR - Vampire Race
; RIVS - Vampire Spells
; DMXL - Dragon Mount No Land List
; PCMD - Player Can Mount Dragon Here List
; FMYS - Flying Mount - Allowed Spells
; FMNS - Flying Mount - Disallowed Spells
; MNT2 - Keyword - Mount
; AIVC - Verlet Cape
; FTNP - Furniture Test NPC
; COEX - Keyword - Conditional Explosion
; VFNC - Vampire Feed No Crime Faction
; KWSP - Skyrim - Worldspace
; ALBM - Keyword - Armor Material Light Bonemold
; ALCH - Keyword - Armor Material Light Chitin
; ALNC - Keyword - Armor Material Light Nordic
; ALSM - Keyword - Armor Material Light Stalhrim
; FMFF - Flying Mount - Fly Fast Worldspaces
; AHBM - Keyword - Armor Material Heavy Bonemold
; AHCH - Keyword - Armor Material Heavy Chitin
; AHNC - Keyword - Armor Material Heavy Nordic
; AHSM - Keyword - Armor Material Heavy Stalhrim
; WPNC - Keyword - Weapon Material Nordic
; WPSM - Keyword - Weapon Material Stalhrim