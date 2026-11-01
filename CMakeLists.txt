cmake_minimum_required(VERSION 3.21)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY "BOTH")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE "BOTH")

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

project(globed2 VERSION 1.0.0)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# these have to come early, cant be in geobuild
set(ARC_FEATURE_NET ON CACHE BOOL "" FORCE)
set(GEODE_CODEGEN_EXTRA_ARGS "--skip-pugixml" CACHE STRING "" FORCE)
# NOTE: was previously pinned to an old asp2 commit (#a2ddce1) for the 2.2081/v5.11.2
# SDK. That commit predates asp/fs/Path.hpp, which the nightly Geode SDK (targeting
# GD 2.2091) requires, so let Geode's own default (asp2@1.5.3) apply instead.
set(GEODE_NO_PUGIXML_HEADER ON CACHE BOOL "" FORCE)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Work around an upstream geode-sdk/bindings bug (see geode-sdk/bindings#1390, unmerged):
# on iOS, the generated GeneratedSource.cpp references FMOD C types (FMOD_RESULT, etc.)
# without including an FMOD header. The fix is to force-include the FMOD C++ header,
# which is already on GeodeBindings' include path (used elsewhere e.g. mapping/Sound.hpp).
if (("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS) AND TARGET GeodeBindings)
    target_compile_options(GeodeBindings PRIVATE "-include" "Geode/fmod/fmod.hpp")
endif()

## Download net libs (ngtcp2, c-ares, openssl, zstd)
include(cmake/DownloadNetLibs.cmake)

CPMAddPackage("gh:dankmeme01/geobuild#e23beaf")
include("${CMAKE_CURRENT_BINARY_DIR}/geobuild-gen.cmake")
