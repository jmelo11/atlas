cmake_minimum_required(VERSION 3.3.0)
project(AtlasPython VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.cpp")
file(GLOB INCLUDES "*.hpp")

find_package(pybind11 REQUIRED)
find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

pybind11_add_module(${PROJECT_NAME} MODULE ${SOURCES} ${INCLUDES})
link_directories("${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}")
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib Boost::boost pybind11::module)

#get ql
get_target_property(QUANTLIB_LIB_NAME QuantLib::QuantLib IMPORTED_LOCATION_RELEASE)


target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ${QuantLib_INCLUDE_DIRS}
)

if (BUILD_WITH_AD)
  set(PYTHON_LIB_NAME "atlas-finance")
  set(AD_LIB_NAME "")
else()
  set(PYTHON_LIB_NAME "atlas-finance-noad")
  set(AD_LIB_NAME "")
  set(NUM_TYPE "double")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
               ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
               @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
                ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
                @ONLY)

# set path and execute pip install .

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release) # You can change "Release" to your desired default configuration
endif()

add_custom_target(install-python
  COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}" ${PYTHON_EXECUTABLE} -m pip install . WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${PROJECT_NAME}
  COMMENT "Installing python package"
  USES_TERMINAL
)
