cmake_minimum_required(VERSION 3.21)
project(BL2_PATCH CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(XdkXex)

add_subdirectory(${CMAKE_SOURCE_DIR}/../../XBOX360_SDK_21256.3 ${CMAKE_BINARY_DIR}/xdk)
add_library(XboxPlatform INTERFACE
        src/core/title/RegisteredTitle.h
        src/core/title/BL2Title.h
        src/core/title/BL2Title.cpp)

target_include_directories(XboxPlatform
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
        $<INSTALL_INTERFACE:platform
)


add_xex(BL2_PATCH
    TYPE       DLL
    SOURCES    src/main.cpp src/core/title/TitleManager.cpp
    ENTRY      DllMain
    CONFIG     "${CMAKE_SOURCE_DIR}/Application.xml"
    USE_XKELIB ON
    XKELIB_DIR "C:/Users/pixel/Documents/360/Xkelib/xkelib"
)

target_link_libraries(BL2_PATCH
        PRIVATE
        XboxSDK
        XboxPlatform
)

