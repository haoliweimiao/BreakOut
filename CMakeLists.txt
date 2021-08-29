# CMakeLists.txt
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME BreakOut)
# 项目信息
project(${PROJECT_NAME})

set(VERSION "0.1.16")

add_definitions("-Wall -g")

# 设置C++标准为C++11
set(CMAKE_CXX_STANDARD 11)

# 设置存放编译时产生的链接库输出到lib文件夹下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置输出文件路径为output文件夹下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

# include路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include )

# 把 第三方 库 的 .a 文件和 .h 文件放到 代码的 lib 文件夹
link_directories(
    ${PROJECT_SOURCE_DIR}/lib)

# add_library(glfw STATIC IMPORTED)
# set_property(TARGET glfw PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libglfw3.a ${PROJECT_SOURCE_DIR}/lib/libglfw3dll.a)


file(GLOB SRC 
    ${PROJECT_SOURCE_DIR}/src/*.cpp 
    ${PROJECT_SOURCE_DIR}/src/*.c
    )
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC src)

# 输出 BreakOut.exe 与 vscode 配置保持一致，即可进行调试
target_link_libraries(
    # 项目名称
    ${PROJECT_NAME} 
    # 链接的静态库
    libglfw3.a 
    libglfw3dll.a 
    libglad.a
    libfreetype.a
    )