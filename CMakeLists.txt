# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(Dartomat)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Core Gui Multimedia Widgets SvgWidgets TextToSpeech REQUIRED)

include_directories(inc)

set(INCLUDES
    inc/add_players_dialog.h
    inc/checkout_lists.h
    inc/cricket_class.h
    inc/cricket_input.h
    inc/cricket_mainwindow.h
    inc/dartboard_base.h
    inc/dartboard_cricket.h
    inc/dartboard_segment.h
    inc/dartboard_view.h
    inc/dartboard_x01.h
    inc/dartomat_mainwindow.h
    inc/game_data_handler.h
    inc/game_data_model.h
    inc/groupbox_cricket.h
    inc/groupbox_x01.h
    inc/player_active_button.h
    inc/playerclass.h
    inc/settings.h
    inc/spinbox.h
    inc/stats_window.h
    inc/x01_mainwindow.h
	)
 
 set(SOURCES
    src/add_players_dialog.cpp
    src/cricket_class.cpp
    src/cricket_input.cpp
    src/cricket_mainwindow.cpp
    src/dartboard_base.cpp
    src/dartboard_cricket.cpp
    src/dartboard_segment.cpp
    src/dartboard_view.cpp
    src/dartboard_x01.cpp
    src/dartomat_mainwindow.cpp
    src/game_data_handler.cpp
    src/game_data_model.cpp
    src/groupbox_cricket.cpp
    src/groupbox_x01.cpp
    src/main.cpp
    src/player_active_button.cpp
    src/playerclass.cpp
    src/spinbox.cpp
    src/stats_window.cpp
    src/x01_mainwindow.cpp
	src/add_players_dialog.ui
    src/cricket_input.ui
    src/cricket_mainwindow.ui
    src/dartomat_mainwindow.ui
    src/groupbox_cricket.ui
    src/groupbox_x01.ui
    src/stats_window.ui
    src/x01_mainwindow.ui
	)

add_executable(${CMAKE_PROJECT_NAME} ${INCLUDES} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::SvgWidgets Qt6::TextToSpeech)
