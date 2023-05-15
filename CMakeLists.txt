cmake_minimum_required(VERSION 3.24)
cmake_policy(SET CMP0077 NEW)
project(myna)

option(REMOTE_LIBSTARLING "Obtain remote libstarling and statically link instead of using local shared library" OFF)

# set up wxWidgets dependency
set(wxWidgets_USE_DEBUG CMAKE_BUILD_TYPE)
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_USE_STATIC OFF)
find_package(wxWidgets REQUIRED gl core base xrc)

# set up libstarling dependency
if(REMOTE_LIBSTARLING)
    include(FetchContent)
    FetchContent_Declare(
        starling
        GIT_REPOSITORY https://github.com/chickazee4/starling
        GIT_TAG main
    )
    set(LIBONLY ON)
    set(STATICLIBS ON)
    FetchContent_MakeAvailable(starling)
else()
    find_library(
        starling REQUIRED
        NAMES libstarling.so libstarling.a
        PATHS /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /lib /lib64
        PATH_SUFFIXES starling
    )
endif()

# generate window code
add_custom_command(
    OUTPUT wxUI.cpp
    COMMAND wxrc -c ${CMAKE_CURRENT_SOURCE_DIR}/gui/MainFrame.xrc ${CMAKE_CURRENT_SOURCE_DIR}/gui/MetadataDialog.xrc -o ${CMAKE_CURRENT_BINARY_DIR}/wxUI.cpp
    DEPENDS gui/MainFrame.xrc gui/MetadataDialog.xrc
)

include_directories(PRIVATE include ${wxWidgets_INCLUDE_DIRS})
add_executable(myna src/myna.cpp src/MainFrame.cpp src/MetadataDialog.cpp wxUI.cpp)
include(${wxWidgets_USE_FILE})
target_link_libraries(myna ${wxWidgets_LIBRARIES} starling)
install(TARGETS myna)
