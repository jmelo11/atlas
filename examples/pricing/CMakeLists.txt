cmake_minimum_required(VERSION 3.10)

project(
  AtlasPricingExample
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
                                                  "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)


