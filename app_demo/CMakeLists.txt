cmake_minimum_required(VERSION 3.10)
#导入cmake配置文件
include(arm-linux-setup.cmake)
#工程创建
project(LVGL_UI)

#配置输出目录
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/release)

# 递归搜索匹配源码目录和其子目录下的.cpp文件
FILE(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# 头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dspread
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lvgl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lvgl/porting
    ${CMAKE_CURRENT_SOURCE_DIR}/src/business
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    ${CMAKE_CURRENT_SOURCE_DIR}/src/http
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ota
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# 链接库目录
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib_pub
)

#将生成的lib 可执行文件放在各自目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)

#添加可执行目标
add_executable( ${PROJECT_NAME} ${SRCS})

#链接动态库
# target_link_libraries(${PROJECT_NAME} PUBLIC binder log pos_sdk pthread dl iconv crypto ssl sqlite3 pos_devapi posmui mxml rt xutilmate APHLang freetype Common qUICS UICS)
target_link_libraries(${PROJECT_NAME} PUBLIC binder log pos_sdk pthread dl iconv crypto ssl sqlite3 pos_devapi posmui mxml rt xutilmate freetype APHLang asound kernel emvl2 entrypoint amex bancomat dpas jcb paypass paywave pure quics rupay mir interac kernelins)