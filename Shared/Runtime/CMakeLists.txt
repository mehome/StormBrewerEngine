cmake_minimum_required(VERSION 3.1.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/External
                    ${PROJECT_SOURCE_DIR}/StormTech
                    ${PROJECT_SOURCE_DIR}/Tools
                    ${PROJECT_SOURCE_DIR}/Shared
                    ${PROJECT_SOURCE_DIR}/Project
                    ${PROJECT_SOURCE_DIR}/Lobby)

set(CMAKE_CXX_STANDARD 17)

include("${PROJECT_SOURCE_DIR}/CMake/cotire.cmake")

set(GENERIC_SRC_Runtime 
            Anchor/AnchorDef.refl.cpp
            Anchor/AnchorTypeDatabase.cpp
            Asset/Asset.cpp
            Atlas/AtlasDef.refl.cpp
            Atlas/AtlasResource.cpp
            BinaryResource/BinaryResourceBase.cpp
            BinaryResource/BinaryResourceManager.cpp
            BinaryResource/BinaryResourceReferenceBase.cpp
            BinaryResource/GenericBinaryResource.cpp
            Collision/CollisionDatabase.cpp
            Collision/DynamicCollisionDatabase.cpp
            Collision/StaticCollisionDatabase.cpp
            Component/ComponentInitData.refl.cpp
            Component/ComponentInitDataTypeDatabase.cpp
            Config/ConfigManager.cpp
            DocumentResource/DocumentResourceBase.cpp
            DocumentResource/DocumentResourceManager.cpp
            DocumentResource/DocumentResourceReferenceBase.cpp
            Entity/EntityDef.refl.cpp
            Entity/EntityResource.cpp
            FrameData/FrameData.refl.cpp
            FrameData/FrameDataExtract.cpp
            Map/MapCollision.cpp
            Map/MapDef.refl.cpp
            Map/MapEffectLayerDef.refl.cpp
            Map/MapEffectLayerTypeDatabase.cpp
            Map/MapHandles.refl.cpp
            Map/MapPropertiesDef.refl.cpp
            Map/MapPropertiesTypeDatabase.cpp
            Map/MapResource.cpp
            Mover/Mover.cpp
            Path/PathDef.refl.cpp
            Path/PathTypeDatabase.cpp
            Runtime.cpp
            RuntimeCommon.cpp
            Script/ScriptResource.cpp
            ServerObject/ServerObject.cpp
            ServerObject/ServerObjectDef.refl.cpp
            ServerObject/ServerObjectEventDispatch.cpp
            ServerObject/ServerObjectEventSystem.cpp
            ServerObject/ServerObjectHandle.cpp
            ServerObject/ServerObjectInitData.refl.cpp
            ServerObject/ServerObjectInitDataTypeDatabase.cpp
            ServerObject/ServerObjectManager.cpp
            ServerObject/ServerObjectResource.cpp
            ServerObject/ServerObjectSerialize.cpp
            ServerObject/ServerObjectSystem.cpp
            ServerObject/ServerObjectUpdate.cpp
            Sprite/SpriteDef.refl.cpp
            Sprite/SpriteResource.cpp
            SpriteBase/SpriteAnimationEventDef.refl.cpp
            SpriteBase/SpriteAnimationEventTypeDatabase.cpp
            SpriteBase/SpriteBaseDef.refl.cpp
            TileSheet/TileSheetDef.refl.cpp
            TileSheet/TileSheetResource.cpp
            UI/UIDef.refl.cpp
            UI/UIResource.cpp
            VisualEffect/VisualEffectDef.refl.cpp
            VisualEffect/VisualEffectResource.cpp
            Volume/VolumeDef.refl.cpp
            Volume/VolumeTypeDatabase.cpp
            #CPP_PLACEHOLDER
            )

set(GENERIC_HEADER_Runtime 
            Anchor/AnchorDef.refl.h
            Anchor/AnchorDef.refl.meta.h
            Anchor/AnchorTypeDatabase.h
            Anchor/AnchorTypeRegister.h
            Animation/AnimationState.h
            Asset/Asset.h
            Atlas/AtlasDef.refl.h
            Atlas/AtlasDef.refl.meta.h
            Atlas/AtlasResource.h
            BinaryResource/BinaryResourceBase.h
            BinaryResource/BinaryResourceLoader.h
            BinaryResource/BinaryResourceManager.h
            BinaryResource/BinaryResourcePtr.h
            BinaryResource/BinaryResourceReference.h
            BinaryResource/BinaryResourceReferenceBase.h
            BinaryResource/GenericBinaryResource.h
            Collision/CollisionDatabase.h
            Collision/CollisionDatabaseResult.h
            Collision/DynamicCollisionDatabase.h
            Collision/IntersectionDatabase.h
            Collision/StaticCollisionDatabase.h
            Component/ComponentInitData.refl.h
            Component/ComponentInitData.refl.meta.h
            Component/ComponentInitDataTypeDatabase.h
            Component/ComponentInitDataTypeDatabaseRegister.h
            Config/ConfigList.h
            Config/ConfigManager.h
            Config/ConfigRegistrationMacros.h
            Config/ConfigResource.h
            DocumentResource/DocumentResourceBase.h
            DocumentResource/DocumentResourceLoader.h
            DocumentResource/DocumentResourceManager.h
            DocumentResource/DocumentResourcePtr.h
            DocumentResource/DocumentResourceReference.h
            DocumentResource/DocumentResourceReferenceBase.h
            Entity/EntityDef.refl.h
            Entity/EntityDef.refl.meta.h
            Entity/EntityResource.h
            Event/Event.h
            Event/EventDispatcher.h
            Event/EventHandler.h
            Event/EventSystem.h
            FrameData/FrameData.refl.h
            FrameData/FrameData.refl.meta.h
            FrameData/FrameDataExtract.h
            GenericResource/GenericResource.h
            Map/MapCollision.h
            Map/MapDef.refl.h
            Map/MapDef.refl.meta.h
            Map/MapEffectLayerDef.refl.h
            Map/MapEffectLayerDef.refl.meta.h
            Map/MapEffectLayerTypeDatabase.h
            Map/MapEffectLayerTypeRegister.h
            Map/MapHandles.refl.h
            Map/MapHandles.refl.meta.h
            Map/MapMetaExtractor.h
            Map/MapPropertiesDef.refl.h
            Map/MapPropertiesDef.refl.meta.h
            Map/MapPropertiesTypeDatabase.h
            Map/MapPropertiesTypeRegister.h
            Map/MapResource.h
            Map/MapTile.h
            Map/MapTileJson.h
            Mover/Mover.h
            Path/PathDef.refl.h
            Path/PathDef.refl.meta.h
            Path/PathTypeDatabase.h
            Path/PathTypeRegister.h
            Runtime.h
            RuntimeCommon.h
            Script/ScriptResource.h
            ServerObject/ServerObject.h
            ServerObject/ServerObjectComponent.h
            ServerObject/ServerObjectComponentFuncs.h
            ServerObject/ServerObjectComponentSerialize.h
            ServerObject/ServerObjectDef.refl.h
            ServerObject/ServerObjectDef.refl.meta.h
            ServerObject/ServerObjectEventDispatch.h
            ServerObject/ServerObjectEventRegister.h
            ServerObject/ServerObjectEventSystem.h
            ServerObject/ServerObjectHandle.h
            ServerObject/ServerObjectInitData.refl.h
            ServerObject/ServerObjectInitData.refl.meta.h
            ServerObject/ServerObjectInitDataTypeDatabase.h
            ServerObject/ServerObjectManager.h
            ServerObject/ServerObjectMetaFuncs.h
            ServerObject/ServerObjectOverlapRegistrationMacros.h
            ServerObject/ServerObjectRegistrationMacros.h
            ServerObject/ServerObjectResource.h
            ServerObject/ServerObjectSerialize.h
            ServerObject/ServerObjectSystem.h
            ServerObject/ServerObjectTypeInfo.h
            ServerObject/ServerObjectUpdate.h
            ServerObject/ServerTypesJson.h
            ServerObject/ServerTypesNet.h
            Sprite/SpriteDef.refl.h
            Sprite/SpriteDef.refl.meta.h
            Sprite/SpriteResource.h
            SpriteBase/SpriteAnimationEventDef.refl.h
            SpriteBase/SpriteAnimationEventDef.refl.meta.h
            SpriteBase/SpriteAnimationEventRegister.h
            SpriteBase/SpriteAnimationEventTypeDatabase.h
            SpriteBase/SpriteBaseDef.refl.h
            SpriteBase/SpriteBaseDef.refl.meta.h
            TileSheet/TileSheetDef.refl.h
            TileSheet/TileSheetDef.refl.meta.h
            TileSheet/TileSheetResource.h
            UI/UIDef.refl.h
            UI/UIDef.refl.meta.h
            UI/UIResource.h
            VisualEffect/VisualEffectDef.refl.h
            VisualEffect/VisualEffectDef.refl.meta.h
            VisualEffect/VisualEffectResource.h
            Volume/VolumeDef.refl.h
            Volume/VolumeDef.refl.meta.h
            Volume/VolumeTypeDatabase.h
            Volume/VolumeTypeRegister.h
            #HEADER_PLACEHOLDER
            )

set(GENERIC_REFL_Runtime 
            Anchor/AnchorDef.refl.h
            Atlas/AtlasDef.refl.h
            Component/ComponentInitData.refl.h
            Entity/EntityDef.refl.h
            FrameData/FrameData.refl.h
            Map/MapDef.refl.h
            Map/MapEffectLayerDef.refl.h
            Map/MapHandles.refl.h
            Map/MapPropertiesDef.refl.h
            Path/PathDef.refl.h
            ServerObject/ServerObjectDef.refl.h
            ServerObject/ServerObjectInitData.refl.h
            Sprite/SpriteDef.refl.h
            SpriteBase/SpriteAnimationEventDef.refl.h
            SpriteBase/SpriteBaseDef.refl.h
            TileSheet/TileSheetDef.refl.h
            UI/UIDef.refl.h
            VisualEffect/VisualEffectDef.refl.h
            Volume/VolumeDef.refl.h
            #REFL_PLACEHOLDER
            )

if(GENERATE_REFL)
  foreach(REFL_FILE ${GENERIC_REFL_Runtime})
    string(REPLACE ".refl.h" ".refl.meta.h" META_FILE ${REFL_FILE})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${META_FILE}
                       COMMAND stormrefl ${CMAKE_CURRENT_SOURCE_DIR}/${REFL_FILE} -- -DSTORM_REFL_PARSE -D_CRT_SECURE_NO_WARNINGS -std=c++17 -x c++ -Wno-pragma-once-outside-header -I${CMAKE_CURRENT_SOURCE_DIR} -I${PROJECT_SOURCE_DIR} -I${PROJECT_SOURCE_DIR}/StormTech -I${PROJECT_SOURCE_DIR}/Tools -I${PROJECT_SOURCE_DIR}/Shared -I${PROJECT_SOURCE_DIR}/Project -I${PROJECT_SOURCE_DIR}/Lobby -I${PROJECT_SOURCE_DIR}/External -I${CLANG_HEADER_PATH} -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
                       MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${REFL_FILE}
                       IMPLICIT_DEPENDS CXX ${CMAKE_CURRENT_SOURCE_DIR}/${REFL_FILE})
  endforeach()
endif()

add_library(Runtime STATIC ${GENERIC_SRC_Runtime} ${GENERIC_HEADER_Runtime})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${GENERIC_SRC_Runtime} ${GENERIC_HEADER_Runtime})

set_target_properties(Runtime PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT RuntimeCommon.h)

if(GENERATE_PCH)
  cotire(Runtime)
endif()
