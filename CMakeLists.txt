cmake_minimum_required(VERSION 3.5)

project(extensible-qt6-app-boilerplate VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES extensible-qt6-app-boilerplate_en_GB.ts)

set(PROJECT_SOURCES
        appimpl.cpp
        appimpl.h
        appinterface.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        pythoninclude.h
        pythoninstance.cpp
        pythoninstance.h
        pythonmoduleapp.cpp
        pythonmoduleapp.h
        pythonmoduleredirect.cpp
        pythonmoduleredirect.h
        pythonutils.cpp
        pythonutils.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(extensible-qt6-app-boilerplate
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET extensible-qt6-app-boilerplate APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(extensible-qt6-app-boilerplate SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(extensible-qt6-app-boilerplate
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(extensible-qt6-app-boilerplate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(extensible-qt6-app-boilerplate PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Avoid clash between Qt and Python (see https://stackoverflow.com/questions/15078060/embedding-python-in-qt-5)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(MIN_PYTHON_VERSION "3.7")
find_package(Python ${MIN_PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${Python_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE
        ${Python_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Python_LIBRARIES})

install(TARGETS extensible-qt6-app-boilerplate
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(extensible-qt6-app-boilerplate)
endif()
