cmake_minimum_required(VERSION 3.16.0)
project(AtlasPython VERSION 2.0.0)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
file(GLOB INCLUDES "*.hpp")

find_package(pybind11 REQUIRED)
find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)
find_package(XAD REQUIRED)
find_package(Atlas REQUIRED)

pybind11_add_module(${PROJECT_NAME} MODULE ${SOURCES} ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib Boost::boost XAD::xad Atlas::Atlas pybind11::module)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${QuantLib_INCLUDE_DIRS}
    ${XAD_INCLUDE_DIRS}
    ${Atlas_INCLUDE_DIRS}
)
 
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
               ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
               @ONLY)