cmake_minimum_required(VERSION 3.8)

project(dartomato_test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Qt6 COMPONENTS Core Gui Multimedia Widgets SvgWidgets Sql Charts Test REQUIRED)

enable_testing()

include_directories(../inc)

set(INCLUDES
    fakes.h
    ../inc/player_list_model.h
    ../inc/game_data_handler.h
    ../inc/checkout_lists.h
    ../inc/cricket_class.h
    ../inc/cricket_input.h
    ../inc/cricket_mainwindow.h
    ../inc/dartboard_base.h
    ../inc/dartboard_cricket.h
    ../inc/dartboard_segment.h
    ../inc/dartboard_view.h
    ../inc/dartboard_x01.h
    ../inc/dartomat_mainwindow.h
    ../inc/groupbox_cricket.h
    ../inc/groupbox_x01.h
    ../inc/player_active_button.h
    ../inc/x01_class.h
    ../inc/settings.h
    ../inc/spinbox.h
    ../inc/stats_window_if.h
    ../inc/stats_window_x01.h
    ../inc/stats_window_cricket.h
    ../inc/x01_mainwindow.h
    ../inc/add_players_dialog.h
    ../inc/leg_stats_x01_model.h
    ../inc/leg_stats_cricket_model.h
    ../inc/leg_scores_x01_model.h
    ../inc/leg_scores_cricket_model.h
    ../inc/global_game_stats_x01_model.h
    ../inc/global_game_stats_cricket_model.h
    ../inc/global_score_stats_x01_model.h
    ../inc/global_segment_stats_x01_model.h
    ../inc/global_segment_stats_cricket_model.h
    ../inc/game_history_dialog.h
    ../inc/game_history_model.h
    ../inc/game_history_table_view.h
    ../inc/game_results_model.h
    ../inc/alltime_stats_dialog.h
    ../inc/alltime_player_stats_model.h
    )

set(SOURCES
    main.cpp
    ../src/add_players_dialog.cpp
    ../src/cricket_class.cpp
    ../src/cricket_input.cpp
    ../src/cricket_mainwindow.cpp
    ../src/dartboard_base.cpp
    ../src/dartboard_cricket.cpp
    ../src/dartboard_segment.cpp
    ../src/dartboard_view.cpp
    ../src/dartboard_x01.cpp
    ../src/dartomat_mainwindow.cpp
    ../src/game_data_handler.cpp
    ../src/player_list_model.cpp
    ../src/groupbox_cricket.cpp
    ../src/groupbox_x01.cpp
    ../src/main.cpp
    ../src/player_active_button.cpp
    ../src/x01_class.cpp
    ../src/spinbox.cpp
    ../src/stats_window_if.cpp
    ../src/stats_window_x01.cpp
    ../src/stats_window_cricket.cpp
    ../src/x01_mainwindow.cpp
    ../src/leg_stats_x01_model.cpp
    ../src/leg_stats_cricket_model.cpp
    ../src/leg_scores_x01_model.cpp
    ../src/leg_scores_cricket_model.cpp
    ../src/global_game_stats_x01_model.cpp
    ../src/global_game_stats_cricket_model.cpp
    ../src/global_score_stats_x01_model.cpp
    ../src/global_segment_stats_x01_model.cpp
    ../src/global_segment_stats_cricket_model.cpp
    ../src/game_history_dialog.cpp
    ../src/game_history_model.cpp
    ../src/game_results_model.cpp
    ../src/alltime_stats_dialog.cpp
    ../src/alltime_player_stats_model.cpp
    ../src/add_players_dialog.ui
    ../src/cricket_input.ui
    ../src/cricket_mainwindow.ui
    ../src/dartomat_mainwindow.ui
    ../src/groupbox_cricket.ui
    ../src/groupbox_x01.ui
    ../src/x01_mainwindow.ui
    ../src/game_history_dialog.ui
    ../src/stats_window_x01.ui
    ../src/stats_window_cricket.ui
    ../src/alltime_stats_dialog.ui
    )

add_definitions(-DTESTING)

qt_add_executable(dartomato_test ${SOURCES} ${INCLUDES})

set_target_properties(dartomato_test PROPERTIES
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(dartomato_test
  PUBLIC
    GTest::gtest_main
    gmock
  PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::SvgWidgets
    Qt6::Sql
    Qt6::Charts)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}../../bin/)

install(CODE "
    file(GLOB_RECURSE QT_LIBS \${CMAKE_INSTALL_PREFIX}/*.dll)
    foreach(LIB \${QT_LIBS})
        file(INSTALL \${LIB} DESTINATION \${CMAKE_CURRENT_BINARY_DIR}/tests)
    endforeach()
")

include(GoogleTest)
gtest_discover_tests(dartomato_test DISCOVERY_MODE PRE_TEST)
