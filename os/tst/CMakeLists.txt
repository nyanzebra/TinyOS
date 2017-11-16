cmake_minimum_required(VERSION 3.9)

enable_testing()

# Cmake module path (if exists)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)

# We probably don't want this to run on every build.
option(COVERALLS "Generate coveralls data" OFF)

if (COVERALLS)
    include(Coveralls)
endif()

set(UNIX_COMPILER "g++")
set(OSX_COMPILER "clang++")
set(MSVC_COMPILER "msvc")

set(CXX_WARNING_LEVEL "-Wall")
# add modules later...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++1z -ftest-coverage -fprofile-arcs ${CXX_WARNING_LEVEL}" CACHE STRING "CXX flags" FORCE)
set(CMAKE_CXX_STANDARD "17" CACHE STRING "Default standard CXX")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Default disallow extensions")
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} CXX_STD_17" CACHE STRING "Default CXX compile features")
set(CMAKE_CXX_STANDARD_REQUIRED OFF CACHE BOOL "Default standard not required")

if(APPLE)
  message("setting compiler to clang")
  set(CMAKE_CXX_COMPILER "${OSX_COMPILER}" CACHE STRING "Default CXX compiler")
elseif(UNIX)
  message("setting compiler to g++")
  set(CMAKE_CXX_COMPILER "${UNIX_COMPILER}" CACHE STRING "Default CXX compiler")
elseif(MSVC or WIN32)
  message("setting compiler to msvc")
  set(CMAKE_CXX_COMPILER "${MSVC_COMPILER}" CACHE STRING "Default CXX compiler")
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-src)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-build)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()
message("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/../kern/")

file(GLOB SRCS *.cpp)
file(GLOB KERNEL_HEADERS ../kern/*.hpp)
set(SRCS ${SRCS} ${KERNEL_HEADERS})
message("Sources: ${SRCS}")
add_executable(testos ${SRCS})

target_link_libraries(testos gtest_main)

if (COVERALLS)

# Create the coveralls target.
coveralls_setup(
		"${SRCS}" # The source files.
		ON                 # If we should upload.
		)
endif()

add_test(testos ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testos)
