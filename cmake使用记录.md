```
cmake_minimum_required (VERSION 3.0)                  cmake_minimum_required()指定最小cmake的版本

project ("ss计划")                                     project工程名

option(DEBUG "option for debug" OFF)                  option选项开关  DEBUG-是否开启
if(DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")                 set设置CMAKE_BUILD_TYPE是DEBUG模式还是Release模式，一个偏重于编译速度
else()
	set(CMAKE_BUILD_TYPE "Release")               一个偏重于性能表现  
endif()

add_subdirectory("Analyzer")                          add_subdirectory包含子路径->去看子路径里的CMakeList.txt文件
```
******************************************************************************************************
```
set(demo_name "demo")                                 设置变量参数，set()

message("start ${demo_name}...")                      屏幕输出message信息

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))                             STREQUAL 用于比较字符串，相同返回 true 
    if (WIN32)                                                                           WIN32平台
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0")                      win32下CMAKE_C_FLAGS_DEBUG用这个作为编译选项生成Makefile，-Wall关闭警告项
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0  -std=c++11")                     -O0 不进行代码优化 ，-O3 三级优化， -g gbd，  -std=c++11  gcc<4.8.5时用std11的特性
    endif()
    Add_Definitions(-D_DEBUG)                                                            -D_DEBUG dubug模式

elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    if (WIN32)
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O0")                    
    else()
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3 -std=c++11")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3  -std=c++11")                       CMAKE_CXX_FLAGS就是gcc加后置参数的吗？
    endif()
else()
endif()
											 PROJECT_SOURCE_DIR 工程的根目录
set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/../bin")                              EXECUTABLE_OUTPUT_PATH 输出路径，###是预定义变量###，目标文件的存储路径

include_directories("${PROJECT_SOURCE_DIR}/../")                                         include_directories 将路径添至头文件搜索路径

if (EXISTS "${PADDLE_DIR}/third_party/snappy/include")		                         EXISTS 判断路径是否存在

find_package(OpenCV REQUIRED PATHS ${OPENCV_DIR}/ NO_DEFAULT_PATH)                       引入外部依赖包，找到别人做好的（OpenCVConfig.cmake，并引用,
										         NO_DEFAULT_PATH选项,那么在搜索时不会附加其它路径。--这个主要是直接指定路径

link_directories("${PROJECT_SOURCE_DIR}/../lib")                                         =  g++命令的-L选项的作用 加载动态库路径

AUX_SOURCE_DIRECTORY(<dir> <variable>) 							 在目录中查找所有源文件，并将名称保存到 SRCS 变量里
AUX_SOURCE_DIRECTORY(. SRCS)

add_executable (${demo_name} ${SRCS})                                                    指定生成目标

target_link_libraries(${demo_name} Analyzer)                                             目标文件与库文件进行链接
```
*******************
```
SET(CUDA_LIB "" CACHE PATH "Location of libraries")                                      缓存一份数据到CMakeCache.txt中         

add_library(${TARGET_NAME} SHARED ${SRCS})                                               生成动态库，指定STATIC，SHARED，MODULE参数用来指定要创建的库的类型。   
```


