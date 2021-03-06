#!/bin/bash

cd $PROJECT_DIR

#rm -rf IncludeOSServer

rm -rf IncludeOSServer/CMake
rm -rf IncludeOSServer/External
rm -rf IncludeOSServer/StormTech
rm -rf IncludeOSServer/Shared
rm -rf IncludeOSServer/Project
rm -rf IncludeOSServer/ProjectSettings
rm -rf IncludeOSServer/Data
rm -rf IncludeOSServer/Data/Assets
rm -rf IncludeOSServer/Data/Config
rm -rf IncludeOSServer/Data/Certs

mkdir -p IncludeOSServer
mkdir -p IncludeOSServer/CMake
mkdir -p IncludeOSServer/External
mkdir -p IncludeOSServer/StormTech
mkdir -p IncludeOSServer/Shared
mkdir -p IncludeOSServer/Project
mkdir -p IncludeOSServer/ProjectSettings
mkdir -p IncludeOSServer/Data
mkdir -p IncludeOSServer/Data/Assets
mkdir -p IncludeOSServer/Data/Config
mkdir -p IncludeOSServer/Data/Certs

cp -p CMake/IncludeOS/* IncludeOSServer/
cp -p CMake/cotire.cmake IncludeOSServer/CMake

cp -rp External/binpack IncludeOSServer/External
cp -rp External/colony IncludeOSServer/External
cp -rp External/glm IncludeOSServer/External
cp -rp External/gsl IncludeOSServer/External
cp -rp External/hash IncludeOSServer/External
cp -rp External/lua IncludeOSServer/External
cp -rp External/mbedtls IncludeOSServer/External
cp -rp External/sb IncludeOSServer/External

cp -rp StormTech/StormBehavior IncludeOSServer/StormTech
cp -rp StormTech/StormData IncludeOSServer/StormTech
cp -rp StormTech/StormExpr IncludeOSServer/StormTech
cp -rp StormTech/StormNet IncludeOSServer/StormTech
cp -rp StormTech/StormNetCustomBindings IncludeOSServer/StormTech
cp -rp StormTech/StormSockets IncludeOSServer/StormTech
cp -rp StormTech/StormRefl IncludeOSServer/StormRefl
cp -rp StormTech/StormBootstrap IncludeOSServer/StormTech
cp -rp StormTech/StormSockets IncludeOSServer/StormTech

cp -rp Shared/Foundation IncludeOSServer/Shared
cp -rp Shared/Runtime IncludeOSServer/Shared
cp -rp Shared/EngineStubs IncludeOSServer/Shared

cp -rp Project/Game IncludeOSServer/Project
cp -rp Project/GameShared IncludeOSServer/Project
cp -rp Project/GameServer IncludeOSServer/Project
cp -rp Project/GameServerExe IncludeOSServer/Project

cp -p ProjectSettings/* IncludeOSServer/ProjectSettings

cp -rp Test/Assets/* IncludeOSServer/Data/Assets
cp -rp Test/Config/* IncludeOSServer/Data/Config
cp -rpL /etc/ssl/certs/* IncludeOSServer/Data/Certs

cd IncludeOSServer
boot -b -g .

