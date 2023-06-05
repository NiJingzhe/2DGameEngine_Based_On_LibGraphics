cmake_minimum_required (VERSION 3.8.0)

project ("2DGameEngine")

#根据自己的需要更改编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
add_compile_options(-std=c11)
#根据自己的需要更改目标文件名称
set(TARGET_NAME CyberRunner)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/libgraphics/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/Engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DGameEngine/Engine/components/)
# 以上是有改观2DGameEngine的配置部分，不用改动
# 一下请根据自己的项目文件结构更改
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/demo)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/demo/scene1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/demo/scene2)


#以下：换成你自己需要的游戏源代码路径，此处为自带demo的路径
aux_source_directory(src/demo/ DEMO_LIST)
aux_source_directory(src/demo/scene1/ SCENE1_LIST)
aux_source_directory(src/demo/scene2/ SCENE2_LIST)
#以上：换成你自己需要的游戏源代码路径，此处为自带demo的路径
aux_source_directory(src/2DGameEngine/libgraphics HEAD_LIST)
aux_source_directory(src/2DGameEngine/Engine ENG_LIST)
aux_source_directory(src/2DGameEngine/Engine/components COMP_LIST)
aux_source_directory(src/2DGameEngine/ CONF_LIST)

# 将源代码添加到此项目的可执行文件。

add_executable (${TARGET_NAME} ${HEAD_LIST} ${ENG_LIST} ${CONF_LIST} ${COMP_LIST} ${DEMO_LIST} ${SCENE1_LIST} ${SCENE2_LIST})
target_link_libraries(${TARGET_NAME} winmm.lib)

# TODO: 如有需要，请添加测试并安装目标。
install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/test_env)