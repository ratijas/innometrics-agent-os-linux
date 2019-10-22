cmake_minimum_required(VERSION 3.6)
project(InnoMetrics C CXX)

set(CMAKE_CXX_STANDARD 11)

###############################################################################
##                       Library innometrics_core                            ##
###############################################################################

add_library(innometrics_core
    Sources/measurements_main.h
    Sources/Helpers/XInputListener.cpp
    Sources/Helpers/XInputListener.h
    Sources/BrowserFile.cpp
    Sources/BrowserFile.h
    Sources/Helpers/Helper.cpp
    Sources/Helpers/Helper.h
    Sources/Helpers/SystemInfoHelper.cpp
    Sources/Helpers/SystemInfoHelper.h
    Sources/DbQueries.h
    Sources/ApplicationType.h
    Sources/Helpers/WindowHelper.h
    Sources/Helpers/WindowHelper.cpp
    Sources/WindowInfo.h
    Sources/WindowInfo.cpp
    Sources/send_demon_main.h
    Sources/updater_demon_main.h
    Sources/Helpers/ConfigParser.h
    Sources/Authorization.h
    Sources/Helpers/HelpFunctions.h
    Sources/SettingDialog.h
    Sources/AddTimeFilterDialog.h
    Sources/MainWindow.h
    )

## -pthread
find_package(Threads REQUIRED)

## SQLite
find_package(SQLite3 REQUIRED)

## CURL
find_package(CURL REQUIRED)

## X11
find_package(X11 REQUIRED)
set(X11_EXT_LIBRARIES
    X11::Xi
    X11::Xmu
    )

## Use the package PkgConfig to detect headers/library files
find_package(PkgConfig REQUIRED)

## GTK
pkg_search_module(GTK3 REQUIRED gtk+-3.0)
target_include_directories(innometrics_core PUBLIC ${GTK3_INCLUDE_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

## appindicator
pkg_search_module(APPINDICATOR REQUIRED appindicator3-0.1)

target_link_libraries(innometrics_core
    Threads::Threads
    SQLite::SQLite3
    CURL::libcurl
    ${X11_LIBRARIES}
    ${X11_EXT_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${APPINDICATOR_LIBRARIES}
    )

###############################################################################
##                       Executable  InnoMetrics                             ##
###############################################################################

add_executable(InnoMetrics main.cpp)

target_link_libraries(InnoMetrics innometrics_core)

###############################################################################
##                                packaging                                  ##
###############################################################################

install(TARGETS InnoMetrics DESTINATION bin)

## XXX: it could've been a better place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/innometrics_build/InnoMetrics/")

## copy the config
configure_file(Sources/inno.cfg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/inno.cfg COPYONLY)

set(CPACK_PACKAGE_NAME "InnoMetrics Linux")
set(CPACK_PACKAGE_VERSION "1.0.0.0")

set(CPACK_MONOLITHIC_INSTALL 1)

include(CPack)
