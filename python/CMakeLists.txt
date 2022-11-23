cmake_minimum_required(VERSION 3.0.0)
project(atlaspython VERSION 0.1.0)

include(GNUInstallDirs) # despues de definir el proyecto
set(CMAKE_CXX_STANDARD 20)
set(BUILD_TESTS ON)

file(GLOB SOURCES "*.cpp" "schemas/*.cpp")

file(GLOB INCLUDES "*.hpp" "schemas/*.hpp")

find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} ${INCLUDES} ${SOURCES})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)
find_package(QuantLibParser REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC QuantLibParser::QuantLibParser)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
target_link_libraries(${PROJECT_NAME} PUBLIC pybind11::pybind11)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json_schema_validator)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
