cmake_minimum_required(VERSION 3.10)

project(
  atlaspythontest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

file(GLOB SOURCES "*.cpp")

file(GLOB INCLUDES "*.hpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main)

# internal libs
find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

find_package(QuantLibParser REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC QuantLibParser::QuantLibParser)

message(STATUS "Using this folder for test building: ${CMAKE_SOURCE_DIR}")

target_include_directories(
  ${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include"
                         "${CMAKE_SOURCE_DIR}/python" "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME}
                      PUBLIC "${CMAKE_SOURCE_DIR}/build/libAtlas.dylib")
