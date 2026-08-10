cmake_minimum_required(VERSION 3.19)
project(chatClient LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(chatClient
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwidget.cpp
    mainwidget.h
    mainwidget.ui
    model/data.h
    resource.qrc
)

# 添加资源（关键！）
qt_add_resources(chatClient "app_resources"
    PREFIX "/"
    FILES
        resource/image/logo.png
)
target_link_libraries(chatClient
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS chatClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET chatClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
