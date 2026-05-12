cmake_minimum_required(VERSION 3.16) #要求最低CMake版本，要不然不给你编译

#定义项目，名字choice_question_answer，版本0.1，语言CXX（cpp）
project(choice_question_answer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 组件：Widgets 和 Sql
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Core Core5Compat)



# 列出所有源文件
set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    Question.cpp
    Question.h
    DatabaseManager.h
    DatabaseManager.cpp
)

# 创建可执行文件
qt_add_executable(choice_question_answer
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    QuestionsManager.h
    QuestionsManager.cpp
    JsonManager.h
    JsonManager.cpp
    HomePage.h
    HomePage.cpp
    AnswerPage.h
    AnswerPage.cpp
    MenuBar.cpp
    MenuBar.h
    ImportJsonPage.h
    ImportJsonPage.cpp
    DatabaseSelectWidget.h
    DatabaseSelectWidget.cpp
    StartPage.h
    StartPage.cpp
    MyListWidget.h
    QuizManager.cpp
    QuizManager.h
    CurrentSizeStackedWidget.h
    QPackManager.h
    QPackManager.cpp
    MyUtils.h


)

# 为 target 添加 QUAZIP_STATIC 宏定义
target_compile_definitions(choice_question_answer PRIVATE
    QUAZIP_STATIC
)

# 设置zlib和quazip的头文件和库文件路径
set(ZLIB_ROOT "C:/dev/zlib-install")
set(QUAZIP_ROOT "C:/dev/quazip-1.5") # 指向你的quazip编译输出目录

find_package(ZLIB REQUIRED)

# 添加头文件搜索路径
target_include_directories(choice_question_answer PRIVATE
    ${QUAZIP_ROOT}/include
)

# 链接必要的 Qt 模块（使用 keyword 风格）
target_link_libraries(choice_question_answer PRIVATE
    Qt6::Widgets
    Qt6::Sql
    ${QUAZIP_ROOT}/libquazip1-qt6.a # 或直接链接 libquazip.a
    ZLIB::ZLIB
    Qt6::Core5Compat
)





# Qt for iOS 设置
set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.choice_question_answer)

set_target_properties(choice_question_answer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS choice_question_answer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(choice_question_answer)
endif()
