cmake_minimum_required(VERSION 3.24)
cmake_policy(SET CMP0077 NEW)
project(myna VERSION 0.2)

option(REMOTE_LIBSTARLING "Obtain remote libstarling and statically link instead of using local shared library" OFF)

# cpack details
set(CPACK_PACKAGE_VENDOR "chickazee4")
set(CPACK_PACKAGE_DESCRIPTION "myna is a graphical program for unpacking and viewing Starling linguistic databases.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/chickazee4/myna")
set(CPACK_PACKAGE_ICON gui/icon.png)
set(CPACK_PACKAGE_CHECKSUM MD5)
set(CPACK_PACKAGE_DIRECTORY packaging)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
if(NOT DEFINED CPACK_PACKAGE_CONTACT)
    set(CPACK_PACKAGE_CONTACT "lyndsay@vivaldi.net")
endif()
if(UNIX AND APPLE AND NOT DEFINED CPACK_GENERATOR)
    set(CPACK_GENERATOR "DragNDrop") # is this what it's called internally? nowhere gives a straight answer and i haven't checked on my mac yet
elseif(UNIX AND (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD") AND NOT DEFINED CPACK_GENERATOR)
    set(CPACK_GENERATOR "TXZ" "FreeBSD")
elseif(UNIX AND (${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND NOT DEFINED CPACK_GENERATOR)
    set(CPACK_GENERATOR "TXZ" "DEB")
elseif(UNIX AND NOT DEFINED CPACK_GENERATOR)
    set(CPACK_GENERATOR "TXZ")
endif()
include(CPack)

# set up wxWidgets dependency
set(wxWidgets_USE_DEBUG CMAKE_BUILD_TYPE)
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_USE_STATIC OFF)
find_package(wxWidgets REQUIRED gl core base xrc)

# GNU gettext support
find_package(Intl)

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

if(NOT DEFINED CMAKE_INSTALL_PREFIX AND UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_DATAROOTDIR /usr/share)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_DATAROOTDIR ${CMAKE_INSTALL_PREFIX}/share)
elseif(UNIX AND APPLE)
    set(CMAKE_INSTALL_DATAROOTDIR ${CMAKE_INSTALL_PREFIX}/Contents/Resources)
endif()

include_directories(PRIVATE include ${wxWidgets_INCLUDE_DIRS} ${Intl_INCLUDE_DIRS})
add_executable(myna src/myna.cpp src/MainFrame.cpp src/MetadataDialog.cpp wxUI.cpp)
add_definitions(-DAPPLICATION_DATA="${CMAKE_INSTALL_DATAROOTDIR}")
include(${wxWidgets_USE_FILE})
target_link_libraries(myna ${wxWidgets_LIBRARIES} ${Intl_LIBRARY} starling)

install(TARGETS myna)
if(UNIX AND NOT APPLE) # BSD/Linux
    if(NOT DEFINED CMAKE_INSTALL_PREFIX)
        install(FILES gui/myna.desktop DESTINATION /usr/share/applications)
    else()
        install(FILES gui/myna.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
    endif()
endif()
install(FILES gui/icon.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/myna)
install(FILES gui/icon.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/myna)
