cmake_minimum_required (VERSION 3.8.0)

project ("2DGameEngine")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/libgraphics/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/Engine/components/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/demo)

#以下：换成你自己需要的游戏源代码路径，此处为自带demo的路径
aux_source_directory(src/demo SRC_LIST)
#以上：换成你自己需要的游戏源代码路径，此处为自带demo的路径
aux_source_directory(src/2DGameEngine/libgraphics HEAD_LIST)
aux_source_directory(src/2DGameEngine/Engine ENG_LIST)
aux_source_directory(src/2DGameEngine/Engine/components COMP_LIST)
aux_source_directory(src/2DGameEngine/ CONF_LIST)

# 将源代码添加到此项目的可执行文件。

add_executable (2DGameEngine ${SRC_LIST} ${HEAD_LIST} ${ENG_LIST} ${CONF_LIST} ${COMP_LIST})
target_link_libraries(2DGameEngine winmm.lib)

# TODO: 如有需要，请添加测试并安装目标。
install(TARGETS 2DGameEngine DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)