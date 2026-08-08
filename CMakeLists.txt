cmake_minimum_required(VERSION 3.21)
project(BL2_PATCH CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(XdkXex)
add_subdirectory(${CMAKE_SOURCE_DIR}/../../XBOX360_SDK_21256.3 ${CMAKE_BINARY_DIR}/xdk)
add_subdirectory(third_party/XexUtils)
add_library(XboxPlatform INTERFACE
        src/platform/PlatformServices.h)
add_library(XboxTitles INTERFACE)

target_include_directories(XboxPlatform
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
        $<INSTALL_INTERFACE:platform
)
target_include_directories(XboxTitles
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/core/>
        $<INSTALL_INTERFACE:title
)

file(GLOB_RECURSE BL2_PATCH_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_xex(BL2_PATCH
    TYPE       DLL
    SOURCES    ${BL2_PATCH_SOURCES}
    ENTRY      DllMain
    CONFIG     "${CMAKE_SOURCE_DIR}/Application.xml"
    USE_XKELIB ON
    XKELIB_DIR "C:/Users/pixel/Documents/360/Xkelib/xkelib"
)


target_link_libraries(BL2_PATCH
        PRIVATE
        XexUtils
        XboxSDK
        XboxPlatform
        XboxTitles
)

if(MSVC)
    # Microsoft Visual Studio / XDK compiler
    target_compile_options(BL2_PATCH PRIVATE /EHsc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC or Clang (e.g., MinGW/MSYS2) - exceptions are enabled by default,
    # but you can explicitly ensure standard exception flags if needed:
    target_compile_options(BL2_PATCH PRIVATE -fexceptions)
endif()

