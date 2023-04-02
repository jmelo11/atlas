cmake_minimum_required(VERSION 3.10)

project(
  idea
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)



add_executable(${PROJECT_NAME} main.cpp)
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

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)




