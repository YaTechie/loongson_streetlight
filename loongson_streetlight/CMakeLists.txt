# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.16)

if (UNIX)
	set(VCPKG_ROOT "/home/vcpkg-mips64el-linux-20230516/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
	set(VCPKG_TARGET_TRIPLET "mips64el-linux")
	set(VCPKG_CXX_FLAGS "-mabi=64")
	set(VCPKG_C_FLAGS "-mabi=64")

else ()
	set(VCPKG_ROOT "D:/loongson_vcpkg/vcpkg-x64-windows-20230514/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
	set(VCPKG_TARGET_TRIPLET x64-windows)
endif()

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
 
project ("loongson_streetlight")

message(STATUS "------------ ${PROJECT_NAME} -------------")

option(VERSION "set version" OFF)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILER_TYPE "")
set(OUTPUT_PATH "")

if(WIN32)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(COMPILER_TYPE "msvc")
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(COMPILER_TYPE "msvc")
	else()
		set(COMPILER_TYPE "gcc")
	endif()
	set(OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin_window")
elseif(APPLE)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
		set(COMPILER_TYPE "clang")
	else()
		set(COMPILER_TYPE "gcc")
	endif()
	set(OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin_apple")
elseif(UNIX)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(COMPILER_TYPE "clang")
	else()
		set(COMPILER_TYPE "gcc")
	endif()
	
	if ("$ENV{DEFINE_GAME_BIN}" STREQUAL "")
		set(OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin_linux")
	else()
		set(OUTPUT_PATH $ENV{GAME_BIN})
	endif()

	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpfullversion -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	message(STATUS "cmake version=${CMAKE_VERSION}")

else()
	message(FATAL_ERROR "unknown COMPILER_TYPE = " ${CMAKE_CXX_COMPILER_ID})  
endif() 

if(${COMPILER_TYPE} STREQUAL "msvc")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP /EHsc /bigobj /await:strict /utf-8 /W4 -Zm2000")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

	add_definitions(-DWINVER=0x0601 -D_WIN32_WINNT=0x0601)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
elseif (${COMPILER_TYPE} STREQUAL "clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fcoroutines-ts -stdlib=libc++")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
	endif()	
elseif (${COMPILER_TYPE} STREQUAL "gcc")
	string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
	list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
	list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

	message(STATUS "gcc version=${GCC_MAJOR}.${GCC_MINOR}")

	#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -W -mabi=64 -mxgot")
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mabi=64")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG")
	endif()

	add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

	set(CMAKE_THREAD_LIBS_INIT "-lpthread")
	#set(CMAKE_HAVE_THREADS_LIBRARY 1)
	#set(CMAKE_USE_WIN32_THREADS_INIT 0)
	#set(CMAKE_USE_PTHREADS_INIT 1)
	#set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")  
	set(OUTPUT_PATH "${OUTPUT_PATH}")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")  
	set(OUTPUT_PATH "${OUTPUT_PATH}/Debug")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")  
	set(OUTPUT_PATH "${OUTPUT_PATH}/Release")
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")  
	set(OUTPUT_PATH "${OUTPUT_PATH}/RelWitchDebInfo")
elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")  
	set(OUTPUT_PATH "${OUTPUT_PATH}/MinSizeRel")
else ()  
	message(FATAL_ERROR "unknown CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})  
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
link_directories(${OUTPUT_PATH})

message(STATUS "COMPILER_TYPE: ${COMPILER_TYPE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "OUTPUT_PATH: ${OUTPUT_PATH}")

find_package(fmt CONFIG REQUIRED)
if(fmt_FOUND)
	link_libraries(fmt::fmt)
else()
	message(FATAL_ERROR  "fmt lib not found.")
endif(fmt_FOUND)

find_package(jsoncpp CONFIG REQUIRED)
if(jsoncpp_FOUND)
	link_libraries(JsonCpp::JsonCpp)
else()
	message(FATAL_ERROR  "jsoncpp lib not found.")
endif(jsoncpp_FOUND)

find_package(CURL CONFIG REQUIRED)
if(CURL_FOUND)
	link_libraries(CURL::libcurl)
else()
	message(FATAL_ERROR  "curl lib not found.")
endif(CURL_FOUND)

find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
	link_libraries(OpenSSL::SSL OpenSSL::Crypto)
else()
	message(FATAL_ERROR  "openssl lib not found.")
endif(OpenSSL_FOUND)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/device)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lower)


