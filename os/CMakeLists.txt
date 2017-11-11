cmake_minimum_required(VERSION 3.9)
project(TinyOS)

set(TinyOS_VERSION_MAJOR 1)
set(TinyOS_VERSION_MINOR 0)

#will eventually support Darwin? Windows?
set(CMAKE_SYSTEM_NAME Linux)

# Force external build
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)
	message(FATAL_ERROR "You can not use CMake to build from the root of it's source tree! Remove the CMakeCache.txt file from this directory, then create a separate directory (either below this directory or elsewhere), and then re-run CMake from there.")
endif(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT WIN32)

# Cmake module path (if exists)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(profile_detection)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXCompilerFlag)

set(PLATFORM "pc" CACHE STRING "Default platform" FORCE)
set(ARCHITECTURE "x86_64")
LOAD_PROFILE(${ARCHITECTURE} ${PLATFORM})

file(GLOB KERNEL_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/*")
list(REMOVE_ITEM KERNEL_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/*.cpp")
file(GLOB KERNEL_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "kern/*.cpp")

set(KERNEL "kernel-${ARCHITECTURE}.bin")

set(ISO "${CMAKE_CURRENT_SOURCE_DIR}/build/isofiles/")
set(GRUB "grub-mkrescue")
set(triple ${ARCHITECTURE}-pc-elf) # triple == arch
message("set triple to ${triple}")
set(PC_LINKER "${ARCHITECTURE}-pc-elf-ld")
set(LINK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/kern/${ARCHITECTURE}/${PLATFORM}/layout.ld")

set(UNIX_COMPILER "g++")
set(OSX_COMPILER "clang++")
set(MSVC_COMPILER "msvc")
set(CROSS_COMPILER "${ARCHITECTURE}-pc-elf-g++")

set(CMAKE_LINKER "${PC_LINKER}" CACHE STRING "Default linker" FORCE)
# -n --gc-sections  --build-id=none
set(CXX_MODULE_FLAGS "-fmodules -fmodule-map-file=./modules/modules.map -fimplicit-modules")
set(CXX_WARNING_LEVEL "-Wall")
# -nostartfiles
set(KERNEL_FLAGS "-mno-red-zone -fpermissive -nostdlib -ffreestanding -m64")
# add modules later...
set(CMAKE_CXX_FLAGS_DEFAULT "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_BASE "-std=c++1z -nostdlib ${KERNEL_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_LEVEL} ${CMAKE_CXX_FLAGS_BASE} -c" CACHE STRING "CXX flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BASE}" CACHE STRING "CXX flags" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BASE}" CACHE STRING "CXX flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BASE}" CACHE STRING "CXX flags" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BASE}" CACHE STRING "CXX flags" FORCE)

set(CMAKE_CXX_STANDARD "17" CACHE STRING "Default standard CXX")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Default disallow extensions")
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} CXX_STD_17" CACHE STRING "Default CXX compile features")
set(CMAKE_CXX_STANDARD_REQUIRED OFF CACHE BOOL "Default standard not required")

if(APPLE)
  set(CMAKE_CXX_COMPILER "/usr/bin/${OSX_COMPILER}" CACHE STRING "Default CXX compiler" FORCE)
elseif(UNIX)
  set(CMAKE_CXX_COMPILER "${UNIX_COMPILER}" CACHE STRING "Default CXX compiler" FORCE)
elseif(MSVC or WIN32)
  set(CMAKE_CXX_COMPILER "${MSVC_COMPILER}" CACHE STRING "Default CXX compiler" FORCE)
endif()

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64) #elf64?
enable_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER_LOADED)
  set(CMAKE_ASM_NASM_FLAGS "-Werror -felf64" CACHE STRING "default nasm flags" FORCE)
  message("nasm loaded adding ${CMAKE_CURRENT_SOURCE_DIR}/kern/${ARCHITECTURE}/${PLATFORM}/*.asm")
  set(CMAKE_ASM_NASM_COMPILER "/usr/local/bin/nasm" CACHE STRING "Correct nasm" FORCE)
  file(GLOB ASM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/kern/${ARCHITECTURE}/${PLATFORM}/*.asm")
  set(ASM_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/kern/${ARCHITECTURE}/${PLATFORM}")
  add_library(ASM_OBJECTS OBJECT "${ASM_SOURCES}")
endif(CMAKE_ASM_NASM_COMPILER_LOADED)

add_library(KERNEL_OBJECTS OBJECT ${KERNEL_SOURCES})

set(CMAKE_CXX_COMPILER_TARGET ${triple})
message("setting compiler to ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} -std=c++1z")
#-arch ${ARCHITECTURE}
set(CMAKE_EXE_LINKER_FLAGS "-n --gc-sections --build-id=none -T ${LINK_FILE}" CACHE STRING "default linker flags" FORCE)
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" CACHE STRING "default linker" FORCE)
add_executable(${PROJECT_NAME}.bin $<TARGET_OBJECTS:ASM_OBJECTS> $<TARGET_OBJECTS:KERNEL_OBJECTS>)

set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX COMPILE_FLAGS "")
set_target_properties(${PROJECT_NAME}.bin PROPERTIES LINKER_LANGUAGE CXX PREFX "" SUFFIX "")
include(make_iso)
include(qemu)
