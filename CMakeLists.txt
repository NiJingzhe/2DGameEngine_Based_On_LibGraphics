# CMakeList.txt: BrainFuckCompiler 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.11.0)

# Enable Hot Reload for MSVC compilers if supported.

project ("2DGameEngine")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DEngine/libgraphics/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/2DEngine/)


aux_source_directory(src/demo SRC_LIST)
aux_source_directory(src/2DEngine/libgraphics HEAD_LIST)
aux_source_directory(src/2DEngine ENG_LIST)

add_compile_options(-std=c11)
# 将源代码添加到此项目的可执行文件。

add_executable (2DGameEngine ${SRC_LIST} ${HEAD_LIST} ${ENG_LIST})
target_link_libraries(2DGameEngine winmm.lib)

# TODO: 如有需要，请添加测试并安装目标。
install(TARGETS 2DGameEngine DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)