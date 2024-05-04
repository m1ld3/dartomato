cmake_minimum_required(VERSION 3.8)
project(Dartomato)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Multimedia Widgets SvgWidgets Sql Charts Test REQUIRED)

include_directories(inc)

set(INCLUDES
    inc/player_list_model.h
    inc/game_data_handler.h
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
    inc/groupbox_cricket.h
    inc/groupbox_x01.h
    inc/player_active_button.h
    inc/x01_class.h
    inc/settings.h
    inc/spinbox.h
    inc/stats_window_if.h
    inc/stats_window_x01.h
    inc/stats_window_cricket.h
    inc/x01_mainwindow.h
    inc/add_players_dialog.h
    inc/leg_stats_x01_model.h
    inc/leg_stats_cricket_model.h
    inc/leg_scores_x01_model.h
    inc/leg_scores_cricket_model.h
    inc/global_game_stats_x01_model.h
    inc/global_game_stats_cricket_model.h
    inc/global_score_stats_x01_model.h
    inc/global_segment_stats_x01_model.h
    inc/global_segment_stats_cricket_model.h
    inc/game_history_dialog.h
    inc/game_history_model.h
    inc/game_history_table_view.h
    inc/game_results_model.h
    inc/alltime_stats_dialog.h
    inc/alltime_player_stats_model.h
    inc/mainwindow_if.h
    inc/x01_mainwindow_if.h
    inc/cricket_mainwindow_if.h
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
    src/player_list_model.cpp
    src/groupbox_cricket.cpp
    src/groupbox_x01.cpp
    src/main.cpp
    src/player_active_button.cpp
    src/x01_class.cpp
    src/spinbox.cpp
    src/stats_window_if.cpp
    src/stats_window_x01.cpp
    src/stats_window_cricket.cpp
    src/x01_mainwindow.cpp
    src/leg_stats_x01_model.cpp
    src/leg_stats_cricket_model.cpp
    src/leg_scores_x01_model.cpp
    src/leg_scores_cricket_model.cpp
    src/global_game_stats_x01_model.cpp
    src/global_game_stats_cricket_model.cpp
    src/global_score_stats_x01_model.cpp
    src/global_segment_stats_x01_model.cpp
    src/global_segment_stats_cricket_model.cpp
    src/game_history_dialog.cpp
    src/game_history_model.cpp
    src/game_results_model.cpp
    src/alltime_stats_dialog.cpp
    src/alltime_player_stats_model.cpp
    src/mainwindow_if.cpp
    src/add_players_dialog.ui
    src/cricket_input.ui
    src/cricket_mainwindow.ui
    src/dartomat_mainwindow.ui
    src/groupbox_cricket.ui
    src/groupbox_x01.ui
    src/x01_mainwindow.ui
    src/game_history_dialog.ui
    src/stats_window_x01.ui
    src/stats_window_cricket.ui
    src/alltime_stats_dialog.ui
    )
	
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(COMMIT_HASH 0)	

find_package(Git QUIET)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_TAG_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
	
	message("Git Tag Version: ${GIT_TAG_VERSION}")
	
  string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)(-[0-9]+-g([0-9a-f]+))?" VERSION_REGEX ${GIT_TAG_VERSION})
  if(VERSION_REGEX)
    set(VERSION_MAJOR ${CMAKE_MATCH_1})
    set(VERSION_MINOR ${CMAKE_MATCH_2})
    set(VERSION_PATCH ${CMAKE_MATCH_3})
    
    if(CMAKE_MATCH_4)
      set(COMMIT_HASH ${CMAKE_MATCH_4})
		else()
			set(COMMIT_HASH 0)
    endif()

    add_definitions("-DVERSION_MAJOR=${VERSION_MAJOR}")
    add_definitions("-DVERSION_MINOR=${VERSION_MINOR}")
    add_definitions("-DVERSION_PATCH=${VERSION_PATCH}")
    add_definitions("-DCOMMIT_HASH=${COMMIT_HASH}")    
  endif()
else()
  message("Git not found, version information won't be available.")
endif()

configure_file(resources/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h @ONLY)
configure_file(resources/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)

set(app_icon_resource_windows ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

qt_add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${INCLUDES} resources/resources.qrc ${CMAKE_CURRENT_BINARY_DIR}/version.h ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)

target_include_directories(Dartomato PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::SvgWidgets Qt6::Sql Qt6::Charts)


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/../bin)

install(
  TARGETS ${CMAKE_PROJECT_NAME}
  BUNDLE  DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
  TARGET ${CMAKE_PROJECT_NAME}
  OUTPUT_SCRIPT deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
  )

install(SCRIPT ${deploy_script})

add_subdirectory(tests)


