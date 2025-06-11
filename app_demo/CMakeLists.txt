cmake_minimum_required(VERSION 3.10)
# Import CMake configuration file
include(arm-linux-setup.cmake)
# Create project
project(LVGL_UI)

# Configure output directory
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/release)

# Recursively search for .cpp and .c files in the source directory and its subdirectories
FILE(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Header file directories
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

# Link library directories
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib_pub
)

# Place the generated libraries and executables in their respective directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)

# Add executable target
add_executable( ${PROJECT_NAME} ${SRCS})

# Link dynamic libraries
# target_link_libraries(${PROJECT_NAME} PUBLIC binder log pos_sdk pthread dl iconv crypto ssl sqlite3 pos_devapi posmui mxml rt xutilmate APHLang freetype Common qUICS UICS)
target_link_libraries(${PROJECT_NAME} PUBLIC binder log pos_sdk pthread dl iconv crypto ssl sqlite3 pos_devapi posmui mxml rt xutilmate freetype APHLang asound kernel emvl2 entrypoint amex bancomat dpas jcb paypass paywave pure quics rupay mir interac kernelins)