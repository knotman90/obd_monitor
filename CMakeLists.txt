cmake_minimum_required(VERSION 2.8)

#set(Boost_INCLUDE_DIR /usr/local/src/boost_1_46_1)
#set(Boost_LIBRARY_DIR /usr/local/src/boost_1_46_1/stage/lib)

set(OBD_MONITOR_MAJOR_VERSION 0)
set(OBD_MONITOR_MINOR_VERSION 1)
set(OBD_MONITOR_VERSION
  ${OBD_MONITOR_MAJOR_VERSION}.${OBD_MONITOR_MINOR_VERSION})

project(OBD_MONITOR-${OBD_MONITOR_VERSION}
		LANGUAGES C CXX)

option(BUILD_CLIENT "Build the client"         ON )
option(BUILD_SIMULATOR "Build the simulator"   ON )

find_package(Boost REQUIRED COMPONENTS system filesystem thread regex)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} is too old. Please use a different C++ compiler.")
endif()

IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

if(BUILD_SIMULATOR)
  add_subdirectory(car_simulator)
endif(BUILD_SIMULATOR)

if(BUILD_CLIENT)
  add_subdirectory(obd_reader)
endif(BUILD_CLIENT)
