cmake_minimum_required(VERSION 3.0.0)
project(AtlasPython VERSION 0.1.0)

include(GNUInstallDirs) # despues de definir el proyecto
set(CMAKE_CXX_STANDARD 20)
set(BUILD_TESTS OFF)

file(GLOB SOURCES "*.cpp" "schemas/*.cpp")

file(GLOB INCLUDES "*.hpp" "schemas/*.hpp")

find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} ${INCLUDES} ${SOURCES})

# parent folder
message(STATUS "Using this folder for test building: ${CMAKE_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
                                                  "${CMAKE_SOURCE_DIR}/src")

if(MSVC)
message(STATUS "MSVC: Looking for lib in folder ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/Atlas.lib")
target_link_libraries(${PROJECT_NAME}
                      PUBLIC "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/Atlas.lib")
else()
target_link_libraries(${PROJECT_NAME}
                      PUBLIC "${CMAKE_SOURCE_DIR}/build/libAtlas.dylib")
endif()


target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
target_link_libraries(${PROJECT_NAME} PUBLIC pybind11::pybind11)


if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
