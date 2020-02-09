#/****************************************************************************
# Copyright (c) 2019 Juan Medina
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME lasers-and-bots)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files
list(APPEND GAME_SOURCE
        Classes/misc/LevelManager.cpp
        Classes/laser_and_bots_app.cpp
        Classes/utils/base/nodes/CustomDrawNode.cpp
        Classes/utils/base/sprite/GameObject.cpp
        Classes/utils/base/app/BasicApp.cpp
        Classes/utils/base/scene/TiledScene.cpp
        Classes/utils/base/scene/BasicScene.cpp
        Classes/utils/base/scene/GridScene.cpp
        Classes/utils/audio/AudioHelper.cpp
        Classes/utils/physics/PhysicsGameObject.cpp
        Classes/utils/physics/PhysicsTiledScene.cpp
        Classes/utils/physics/PhysicsShapeCache.cpp
        Classes/utils/controller/InputController.cpp
        Classes/utils/utils.cpp
        Classes/game/box_object.cpp
        Classes/game/robot_object.cpp
        Classes/game/barrel_object.cpp
        Classes/game/harm_object.cpp
        Classes/game/saw_object.cpp
        Classes/game/background_layer.cpp
        Classes/game/switch_object.cpp
        Classes/game/laser_object.cpp
        Classes/game/door_object.cpp
        Classes/game/robot_fragment.cpp
        Classes/game/on_off_object.cpp
        Classes/menu/OptionsMenu.cpp
        Classes/menu/MainMenu.cpp
        Classes/menu/PlayMenu.cpp
        Classes/menu/AboutMenu.cpp
        Classes/menu/LicenseMenu.cpp
        Classes/menu/BasicMenu.cpp
        Classes/menu/CreditsMenu.cpp
        Classes/scenes/game_scene.cpp
        Classes/scenes/LoadingScene.cpp
        Classes/scenes/menu_scene.cpp
        Classes/ui/MessageWindow.cpp
        Classes/ui/OnScreenButton.cpp
        Classes/ui/LevelCompleted.cpp
        Classes/ui/TextToggle.cpp
        Classes/ui/SliderObject.cpp
        Classes/ui/TextButton.cpp
        Classes/ui/ResizableWindow.cpp
        Classes/ui/PauseWindow.cpp
        Classes/ui/VirtualJoyStick.cpp
        Classes/ui/ScrollingText.cpp
        Classes/ui/GameUI.cpp
     )
list(APPEND GAME_HEADER
        Classes/misc/LevelManager.h
        Classes/laser_and_bots_app.h
        Classes/utils/base/nodes/CustomDrawNode.h
        Classes/utils/base/sprite/GameObject.h
        Classes/utils/base/app/BasicApp.h
        Classes/utils/base/scene/BasicScene.h
        Classes/utils/base/scene/GridScene.h
        Classes/utils/base/scene/TiledScene.h
        Classes/utils/audio/AudioHelper.h
        Classes/utils/utils.h
        Classes/utils/physics/PhysicsTiledScene.h
        Classes/utils/physics/PhysicsGameObject.h
        Classes/utils/physics/PhysicsShapeCache.h
        Classes/utils/controller/InputController.h
        Classes/game/switch_object.h
        Classes/game/barrel_object.h
        Classes/game/robot_fragment.h
        Classes/game/robot_object.h
        Classes/game/saw_object.h
        Classes/game/background_layer.h
        Classes/game/laser_object.h
        Classes/game/door_object.h
        Classes/game/on_off_object.h
        Classes/game/box_object.h
        Classes/game/harm_object.h
        Classes/menu/BasicMenu.h
        Classes/menu/AboutMenu.h
        Classes/menu/OptionsMenu.h
        Classes/menu/PlayMenu.h
        Classes/menu/CreditsMenu.h
        Classes/menu/LicenseMenu.h
        Classes/menu/MainMenu.h
        Classes/scenes/game_scene.h
        Classes/scenes/LoadingScene.h
        Classes/scenes/menu_scene.h
        Classes/ui/LevelCompleted.h
        Classes/ui/MessageWindow.h
        Classes/ui/SliderObject.h
        Classes/ui/GameUI.h
        Classes/ui/ResizableWindow.h
        Classes/ui/VirtualJoyStick.h
        Classes/ui/PauseWindow.h
        Classes/ui/TextToggle.h
        Classes/ui/OnScreenButton.h
        Classes/ui/TextButton.h
        Classes/ui/ScrollingText.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -fexceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
