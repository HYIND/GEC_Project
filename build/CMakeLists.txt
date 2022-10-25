cmake_minimum_required (VERSION 3.10.2)

SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_C_COMPILER arm-linux-gcc)

project(myproject C)

include_directories(../include)
aux_source_directory(../src DIR_SRCS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# set(CMAKE_BUILD_TYPE "Debug")

LINK_DIRECTORIES(../libs)
LINK_LIBRARIES(libfont.a)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread -lm -ljpeg -lapi_v4l2_arm1")

add_executable(myproject ${DIR_SRCS})

