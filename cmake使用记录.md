cmake_minimum_required (VERSION 3.0)                  指定最小cmake的版本

project ("ss计划")                                     工程名

option(DEBUG "option for debug" OFF)                  选项开关  DEBUG-是否开启
if(DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")                       设置CMAKE_BUILD_TYPE是DEBUG模式还是Release模式，一个偏重于编译速度
else()
	set(CMAKE_BUILD_TYPE "Release")                     一个偏重于性能表现
endif()

add_subdirectory("Analyzer")                          包含子路径->去看子路径里的CMakeList.txt文件
******************************************************************************************************

set(demo_name "demo")                                 设置变量参数，set()

message("start build ${demo_name}...")
