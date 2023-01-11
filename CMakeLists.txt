cmake_minimum_required(VERSION 3.19)

project ("softver-za-prevoz-putnika")



set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
message(STATUS "Exporting compile_commands.json")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# git
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  message(STATUS "Updating git submodules...")
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    RESULT_VARIABLE UPDATE_SUBMODULES_RESULT
  )
  if(NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update failed!")
  endif()
endif()


#Set flag on linnux
if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#add library folder
set(FTXUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodel/FTXUI)

set (FTXUI_BUILD_DOCS  OFF)
set (FTXUI_BUILD_EXAMPLES  OFF)
set (FTXUI_BUILD_TESTS  OFF)
set (FTXUI_BUILD_TESTS_FUZZER OFF)
set (FTXUI_ENABLE_INSTALL OFF)

add_subdirectory(${FTXUI_DIR})


# exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "source/*.?pp")
add_executable(${PROJECT_NAME} ${SOURCES} "source/main.cpp" "include/gui.h" "source/gui.cpp" "include/voznja.h" "source/voznja.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${FTXUI_DIR}/include src include)


target_link_libraries(${PROJECT_NAME} 
                        PRIVATE ftxui::screen
                        PRIVATE ftxui::dom
  			                PRIVATE ftxui::component)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)