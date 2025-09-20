cmake_minimum_required(VERSION 3.31)

project(A704F_Mouse VERSION 1.0.2 LANGUAGES CXX)
include(FetchContent)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROGRAM_AUTHOR, "AmberFrost")
set(PROGRAM_AUTHOR_EMAIL, "amberfrost@lx862.com")
set(PROGRAM_SRC_URL, "https://github.com/AmberIsFrozen/Instant-A704F-Mouse-Utilities")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/rgbpreview.h
        src/rgbpreview.cpp
        src/custominputhandler.cpp
        src/custominputhandler.h
        src/mousesettings.h
        src/mousesettings.cpp
        src/hidhelper.h
        src/hidhelper.cpp
        resource.qrc
        src/customkeyboarddialog.h
        src/customkeyboarddialog.cpp
        src/customkeyboarddialog.ui
        src/data.cpp
        src/usb_hid_keys.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(A704F_Mouse
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET A704F_Mouse APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(A704F_Mouse SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(A704F_Mouse
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(A704F_Mouse PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(A704F_Mouse PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS A704F_Mouse
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(A704F_Mouse)
endif()

set(BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
  hidapi
  URL      https://github.com/libusb/hidapi/archive/refs/tags/hidapi-0.15.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)


FetchContent_MakeAvailable(hidapi)
target_link_libraries(A704F_Mouse PUBLIC hidapi::hidapi)

set(CMAKE_INSTALL_PREFIX /usr)

install(TARGETS A704F_Mouse DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/51-a704f-usb-gaming-mouse.rules DESTINATION /etc/udev/rules.d)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/img/tray.png RENAME a704f-mouse.png DESTINATION share/pixmaps)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/a704f-mouse.desktop DESTINATION share/applications)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)

# Uncomment this lines to enable shlibs for this project
# set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
# set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)

set(CPACK_DEBIAN_PACKAGE_NAME "instant-a704f-mouse-utilities")

set(CPACK_DEBIAN_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

# Uncomment this lines to use timestamp for the Debian release number
# string(TIMESTAMP NOW "%s")
# set(CPACK_DEBIAN_PACKAGE_RELEASE ${NOW})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mouse Utilities for the Instant A704/A704F Mouse")

set(CPACK_PACKAGE_DESCRIPTION
"This is an unofficial program that can be used to configure the A704/A704F Mouse by Instant Microelectronics.")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROGRAM_AUTHOR} <${PROGRAM_AUTHOR_EMAIL}>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE PROGRAM_SRC_URL)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-base-dev")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA ${CMAKE_CURRENT_SOURCE_DIR}/script/postinst)

include(CPack)
