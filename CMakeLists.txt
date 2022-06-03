cmake_minimum_required (VERSION 3.13)

project(SyncSerialAnalyzer)

add_definitions( -DLOGIC2 )

# enable generation of compile_commands.json, helpful for IDEs to locate include files.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/SyncSerialAnalyzer.cpp
src/SyncSerialAnalyzer.h
src/SyncSerialAnalyzerResults.cpp
src/SyncSerialAnalyzerResults.h
src/SyncSerialAnalyzerSettings.cpp
src/SyncSerialAnalyzerSettings.h
src/SyncSerialSimulationDataGenerator.cpp
src/SyncSerialSimulationDataGenerator.h
)

add_analyzer_plugin(${PROJECT_NAME} SOURCES ${SOURCES})
