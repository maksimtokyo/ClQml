cmake_minimum_required(VERSION 3.5)

project(Cl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sources)

add_subdirectory(external/td)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia NetworkAuth Sql)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appCl
    main.cpp
)

qt_add_qml_module(appCl
    URI Cl
    VERSION 1.0

    QML_FILES
        Main.qml
        qmlfiles/codeview.qml
        qmlfiles/passwordview.qml
        qmlfiles/authorizationreadyview.qml
        qmlfiles/phoneview.qml

    SOURCES
        sources/tgcontroller.h sources/tgcontroller.cpp
        sources/tgclient.h sources/tgclient.cpp
        sources/debugmodel.h sources/debugmodel.cpp
        sources/musiclist.h sources/musiclist.cpp
        sources/geniusclient.h sources/geniusclient.cpp
        sources/dbuser.h sources/dbuser.cpp
        SOURCES sources/cluser.h sources/cluser.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCl PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCl
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCl
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Multimedia
    PRIVATE Td::TdStatic
    PRIVATE Qt6::NetworkAuth
    PRIVATE Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appCl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
