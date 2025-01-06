cmake_minimum_required(VERSION 3.16)

project(Musikspieler VERSION 0.1 LANGUAGES CXX)

# CMake settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt modules
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Sql)

# Define source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mediacontroller.h mediacontroller.cpp
    database.h database.cpp
    playlist.h playlist.cpp
    installerdialog.h installerdialog.cpp installerdialog.ui
    track.cpp
    selecttrackdialog.cpp # Fügen Sie diese Zeile hinzu
)

# Define the target executable/library
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Musikspieler
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Musikspieler SHARED ${PROJECT_SOURCES})
    else()
        add_executable(Musikspieler ${PROJECT_SOURCES})
    endif()
endif()

# Link libraries
target_link_libraries(Musikspieler
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
)

# macOS/iOS bundle settings
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Musikspieler)
endif()
set_target_properties(Musikspieler PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

# Installation settings
include(GNUInstallDirs)
install(TARGETS Musikspieler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Musikspieler)
endif()
