cmake_minimum_required(VERSION 3.10)

project(
  atlasbenchmark
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

file(GLOB SOURCES "*.cpp")

file(GLOB INCLUDES "*.hpp")

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES})

find_package(nanobench REQUIRED)
find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

# parent folder
message(STATUS "Using this folder for test building: ${CMAKE_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
                                                  "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME}
                      PUBLIC "${CMAKE_SOURCE_DIR}/build/libatlas.dylib")

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
target_link_libraries(${PROJECT_NAME} PUBLIC nanobench::nanobench)
