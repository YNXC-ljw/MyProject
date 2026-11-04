cmake_minimum_required(VERSION 3.16)

project(ChatClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

source_group("Source Files" REGULAR_EXPRESSION "\\.(c|cc|cpp|cxx)$")
source_group("Header Files" REGULAR_EXPRESSION "\\.(h|hh|hpp|hxx)$")
source_group("Forms" REGULAR_EXPRESSION "\\.ui$")
source_group("Resources" REGULAR_EXPRESSION "\\.(qrc|png|jpg|jpeg|gif|svg|ico)$")

file(GLOB PB_FILES "./proto/*.proto")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebSockets Protobuf)

set(PROJECT_SOURCES
        main.cpp
        mainwidget.cpp
        mainwidget.h
        mainwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Keep generated Protobuf code in a separate library. App-only changes can
    # then reuse the existing Protobuf archive instead of repacking it.
    add_library(ChatClientProto SHARED)
    qt_add_protobuf(ChatClientProto PROTO_FILES ${PB_FILES})

    # user_qtprotoreg.cpp can exceed the section limit of a regular MinGW
    # COFF object. Qt applies mbig-obj to ChatClientProto, but not to the
    # automatically-created registration object target.
    if(MINGW)
        target_compile_options(ChatClientProto_qtprotoreg PRIVATE "-Wa,-mbig-obj")
    endif()

    target_include_directories(ChatClientProto PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    )
    target_link_libraries(ChatClientProto PUBLIC
        Qt6::Protobuf
    )

    qt_add_executable(ChatClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        model/data.h
        resource.qrc
        sessionfriendarea.h sessionfriendarea.cpp
        debug.h
        sessionshowarea.h sessionshowarea.cpp
        messageeditarea.h messageeditarea.cpp
        selfinfowidget.h selfinfowidget.cpp
        userinfowidget.h userinfowidget.cpp
        sessiondetailwidget.h sessiondetailwidget.cpp
        choosefrienddialog.h choosefrienddialog.cpp
        groupsessiondetailwidget.h groupsessiondetailwidget.cpp
        addfrienddialog.h addfrienddialog.cpp
        historymessagewidget.h historymessagewidget.cpp
        loginwidget.h loginwidget.cpp
        phoneloginwidget.h phoneloginwidget.cpp
        model/datacenter.h model/datacenter.cpp
        Network/netclient.h Network/netclient.cpp
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ChatClient APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ChatClient SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ChatClient
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ChatClient PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network Qt6::WebSockets Qt6::Protobuf)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_link_libraries(ChatClient PRIVATE ChatClientProto)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ChatClient)
endif()
set_target_properties(ChatClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ChatClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ChatClient)
endif()
