cmake_minimum_required(VERSION 3.18)

# ---- Project ----

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/versioning.cmake)

project(
	skse64
	VERSION ${SKSE_VERSION_MAJOR}.${SKSE_VERSION_MINOR}.${SKSE_VERSION_PATCH}
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Build options ----

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

# ---- Dependencies ----

if (NOT TARGET skse64_common)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../skse64_common skse64_common)	# bundled
endif()

if (NOT TARGET xbyak)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../xbyak xbyak)	# bundled
endif()

find_package(common REQUIRED CONFIG)

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
	${PROJECT_NAME}
	FILES
		skse64.cpp
		skse64.def
		skse_version.rc
)

source_group(
	${PROJECT_NAME}/api
	FILES
		GameAPI.cpp
		GameAPI.h
		GameBSExtraData.cpp
		GameBSExtraData.h
		GameCamera.cpp
		GameCamera.h
		GameData.cpp
		GameData.h
		GameEvents.cpp
		GameEvents.h
		GameExtraData.cpp
		GameExtraData.h
		GameFormComponents.cpp
		GameFormComponents.h
		GameForms.cpp
		GameForms.h
		GameHandlers.cpp
		GameHandlers.h
		GameInput.cpp
		GameInput.h
		GameMenus.cpp
		GameMenus.h
		GameObjects.cpp
		GameObjects.h
		GamePathing.cpp
		GameReferences.cpp
		GameReferences.h
		GameResources.cpp
		GameResources.h
		GameRTTI.cpp
		GameRTTI.h
		GameRTTI.inl
		GameSettings.cpp
		GameSettings.h
		GameStreams.cpp
		GameStreams.h
		GameThreads.cpp
		GameThreads.h
		GameTypes.cpp
		GameTypes.h
		GameUtilities.cpp
		GameUtilities.h
		ObScript.cpp
		ObScript.h
)

source_group(
	${PROJECT_NAME}/hooks
	FILES
		Hooks_Camera.cpp
		Hooks_Camera.h
		Hooks_Data.cpp
		Hooks_Data.h
		Hooks_Debug.cpp
		Hooks_Debug.h
		Hooks_Diagnostics.cpp
		Hooks_Diagnostics.h
		Hooks_DirectInput8Create.cpp
		Hooks_DirectInput8Create.h
		Hooks_Event.cpp
		Hooks_Event.h
		Hooks_Gameplay.cpp
		Hooks_Gameplay.h
		Hooks_Handlers.cpp
		Hooks_Handlers.h
		Hooks_Memory.cpp
		Hooks_Memory.h
		Hooks_NetImmerse.cpp
		Hooks_NetImmerse.h
		Hooks_ObScript.cpp
		Hooks_ObScript.h
		Hooks_Papyrus.cpp
		Hooks_Papyrus.h
		Hooks_SaveLoad.cpp
		Hooks_SaveLoad.h
		Hooks_Scaleform.cpp
		Hooks_Scaleform.h
		Hooks_Threads.cpp
		Hooks_Threads.h
		Hooks_UI.cpp
		Hooks_UI.h
)

source_group(
	${PROJECT_NAME}/internal
	FILES
		Colors.cpp
		Colors.h
		CustomMenu.cpp
		CustomMenu.h
		GlobalLocks.cpp
		GlobalLocks.h
		HashUtil.cpp
		HashUtil.h
		InputMap.cpp
		InputMap.h
		InternalSerialization.cpp
		InternalSerialization.h
		InternalTasks.cpp
		InternalTasks.h
		PluginAPI.h
		PluginLoadErrorDialog.rc
		PluginManager.cpp
		PluginManager.h
		Serialization.cpp
		Serialization.h
		Translation.cpp
		Translation.h
)

source_group(
	${PROJECT_NAME}/netimmerse
	FILES
		BSModelDB.cpp
		BSModelDB.h
		NiAdditionalGeometryData.cpp
		NiAdditionalGeometryData.h
		NiControllers.cpp
		NiControllers.h
		NiExtraData.cpp
		NiExtraData.h
		NiGeometry.cpp
		NiGeometry.h
		NiInterpolators.cpp
		NiInterpolators.h
		NiLight.cpp
		NiLight.h
		NiMaterial.cpp
		NiMaterial.h
		NiNodes.cpp
		NiNodes.h
		NiObjects.cpp
		NiObjects.h
		NiProperties.cpp
		NiProperties.h
		NiRenderer.cpp
		NiRenderer.h
		NiRTTI.cpp
		NiRTTI.h
		NiSerialization.cpp
		NiSerialization.h
		NiTextures.cpp
		NiTextures.h
		NiTypes.cpp
		NiTypes.h
)

source_group(
	${PROJECT_NAME}/papyrus/functions
	FILES
		PapyrusActiveMagicEffect.cpp
		PapyrusActiveMagicEffect.h
		PapyrusActor.cpp
		PapyrusActor.h
		PapyrusActorBase.cpp
		PapyrusActorBase.h
		PapyrusActorValueInfo.cpp
		PapyrusActorValueInfo.h
		PapyrusAlias.cpp
		PapyrusAlias.h
		PapyrusAmmo.cpp
		PapyrusAmmo.h
		PapyrusArmor.cpp
		PapyrusArmor.h
		PapyrusArmorAddon.cpp
		PapyrusArmorAddon.h
		PapyrusArt.cpp
		PapyrusArt.h
		PapyrusBook.cpp
		PapyrusBook.h
		PapyrusCamera.cpp
		PapyrusCamera.h
		PapyrusCell.cpp
		PapyrusCell.h
		PapyrusClass.cpp
		PapyrusClass.h
		PapyrusColorForm.cpp
		PapyrusColorForm.h
		PapyrusCombatStyle.cpp
		PapyrusCombatStyle.h
		PapyrusConstructibleObject.cpp
		PapyrusConstructibleObject.h
		PapyrusDefaultObjectManager.cpp
		PapyrusDefaultObjectManager.h
		PapyrusEnchantment.cpp
		PapyrusEnchantment.h
		PapyrusEquipSlot.cpp
		PapyrusEquipSlot.h
		PapyrusFaction.cpp
		PapyrusFaction.h
		PapyrusFlora.cpp
		PapyrusFlora.h
		PapyrusForm.cpp
		PapyrusForm.h
		PapyrusFormList.cpp
		PapyrusFormList.h
		PapyrusGame.cpp
		PapyrusGame.h
		PapyrusGameData.cpp
		PapyrusGameData.h
		PapyrusHeadPart.cpp
		PapyrusHeadPart.h
		PapyrusIngredient.cpp
		PapyrusIngredient.h
		PapyrusInput.cpp
		PapyrusInput.h
		PapyrusKeyword.cpp
		PapyrusKeyword.h
		PapyrusLeveledActor.cpp
		PapyrusLeveledActor.h
		PapyrusLeveledItem.cpp
		PapyrusLeveledItem.h
		PapyrusLeveledSpell.cpp
		PapyrusLeveledSpell.h
		PapyrusLocation.cpp
		PapyrusLocation.h
		PapyrusMagicEffect.cpp
		PapyrusMagicEffect.h
		PapyrusMath.cpp
		PapyrusMath.h
		PapyrusMisc.cpp
		PapyrusMisc.h
		PapyrusModEvent.cpp
		PapyrusModEvent.h
		PapyrusNetImmerse.cpp
		PapyrusNetImmerse.h
		PapyrusObjectReference.cpp
		PapyrusObjectReference.h
		PapyrusPerk.cpp
		PapyrusPerk.h
		PapyrusPotion.cpp
		PapyrusPotion.h
		PapyrusQuest.cpp
		PapyrusQuest.h
		PapyrusRace.cpp
		PapyrusRace.h
		PapyrusReferenceAlias.cpp
		PapyrusReferenceAlias.h
		PapyrusScroll.cpp
		PapyrusScroll.h
		PapyrusShout.cpp
		PapyrusShout.h
		PapyrusSKSE.cpp
		PapyrusSKSE.h
		PapyrusSound.cpp
		PapyrusSound.h
		PapyrusSoundDescriptor.cpp
		PapyrusSoundDescriptor.h
		PapyrusSpawnerTask.cpp
		PapyrusSpawnerTask.h
		PapyrusSpell.cpp
		PapyrusSpell.h
		PapyrusStringUtil.cpp
		PapyrusStringUtil.h
		PapyrusTextureSet.cpp
		PapyrusTextureSet.h
		PapyrusTree.cpp
		PapyrusTree.h
		PapyrusUI.cpp
		PapyrusUI.h
		PapyrusUICallback.cpp
		PapyrusUICallback.h
		PapyrusUtility.cpp
		PapyrusUtility.h
		PapyrusWeapon.cpp
		PapyrusWeapon.h
		PapyrusWeather.cpp
		PapyrusWeather.h
		PapyrusWornObject.cpp
		PapyrusWornObject.h
)

source_group(
	${PROJECT_NAME}/papyrus/vm
	FILES
		PapyrusArgs.cpp
		PapyrusArgs.h
		PapyrusDelayFunctors.cpp
		PapyrusDelayFunctors.h
		PapyrusEventFunctor.h
		PapyrusEvents.cpp
		PapyrusEvents.h
		PapyrusInterfaces.cpp
		PapyrusInterfaces.h
		PapyrusNativeFunctionDef.inl
		PapyrusNativeFunctionDef_Base.inl
		PapyrusNativeFunctions.cpp
		PapyrusNativeFunctions.h
		PapyrusObjects.cpp
		PapyrusObjects.h
		PapyrusValue.cpp
		PapyrusValue.h
		PapyrusVM.cpp
		PapyrusVM.h
)

source_group(
	${PROJECT_NAME}/scaleform
	FILES
		ScaleformAPI.cpp
		ScaleformAPI.h
		ScaleformCallbacks.cpp
		ScaleformCallbacks.h
		ScaleformExtendedData.cpp
		ScaleformExtendedData.h
		ScaleformLoader.cpp
		ScaleformLoader.h
		ScaleformMovie.cpp
		ScaleformMovie.h
		ScaleformState.cpp
		ScaleformState.h
		ScaleformTypes.cpp
		ScaleformTypes.h
		ScaleformValue.cpp
		ScaleformValue.h
		ScaleformVM.cpp
		ScaleformVM.h
)

# ---- Create library ----

add_library(
	${PROJECT_NAME}
	SHARED
	${headers}
	${sources}
	skse64.def
	${skse64_common_SOURCE_DIR}/skse_version.rc
	PluginLoadErrorDialog.rc
)

add_library(skse64::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configuration.cmake)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		OUTPUT_NAME "${PROJECT_NAME}_${RUNTIME_VERSION_MAJOR}_${RUNTIME_VERSION_MINOR}_${RUNTIME_VERSION_PATCH}"
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		RUNTIME
		RUNTIME_VERSION=${RUNTIME_VERSION_PACKED}
)

target_compile_features(
	${PROJECT_NAME}
	PUBLIC
		cxx_std_11
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
		skse64::skse64_common
		Xbyak::xbyak
		xse::common
)

# ---- Configure all targets ----

set_target_properties(
	${PROJECT_NAME}
	skse64_common
	PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# ---- Create an installable target ----

include(GNUInstallDirs)

install(
	TARGETS
		${PROJECT_NAME}
	EXPORT
		${PROJECT_NAME}-targets
)

install(
	FILES
		${headers}
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

configure_file(
	cmake/config.cmake.in
	${PROJECT_NAME}-config.cmake
	@ONLY
)

install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	EXPORT
		${PROJECT_NAME}-targets
	NAMESPACE
		skse64::
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/installation.cmake)
