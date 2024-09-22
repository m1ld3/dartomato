cmake_minimum_required(VERSION 3.8)
project(Dartomato)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/Zc:__cplusplus)
endif()

find_package(Qt6 COMPONENTS Core Gui Multimedia Widgets SvgWidgets Sql Charts Test REQUIRED)

include_directories(inc)

# Collect all source and header files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.ui)
file(GLOB_RECURSE INCLUDES CONFIGURE_DEPENDS inc/*.h)

# Git versioning logic
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

# Generate version header and resource files
configure_file(resources/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h @ONLY)
configure_file(resources/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)

# Add the executable
qt_add_executable(${CMAKE_PROJECT_NAME}
  ${SOURCES}
  ${INCLUDES}
  resources/resources.qrc
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

# Platform-specific settings
if(WIN32)
  # Windows-specific: Set executable properties
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
  set(app_icon_resource_windows ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif()

# Linux-specific settings
if(UNIX AND NOT APPLE)
  # Set executable to be position-independent (for shared libraries)
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Set include directories
target_include_directories(Dartomato PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link required Qt libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Multimedia
  Qt6::SvgWidgets
  Qt6::Sql
  Qt6::Charts
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bin)

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

# workaround for Github for generating the deploy script
if(CMAKE_IS_GITHUB_WORKFLOW AND WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
  install(SCRIPT "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/.qt/deploy_${CMAKE_PROJECT_NAME}_$Release.cmake")
elseif(CMAKE_IS_GITHUB_WORKFLOW AND WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  install(SCRIPT "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/.qt/deploy_${CMAKE_PROJECT_NAME}_$Debug.cmake")
elseif(CMAKE_IS_GITHUB_WORKFLOW AND WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  install(SCRIPT "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/.qt/deploy_${CMAKE_PROJECT_NAME}_$RelWithDebInfo.cmake")
elseif(CMAKE_IS_GITHUB_WORKFLOW AND WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
  install(SCRIPT "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/.qt/deploy_${CMAKE_PROJECT_NAME}_$MinSizeRel.cmake")
else()
  install(SCRIPT ${deploy_script})
endif()
  
# Enable testing
add_subdirectory(tests)
