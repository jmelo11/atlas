cmake_minimum_required(VERSION 3.16.0)
project(AtlasPython VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
file(GLOB INCLUDES "*.hpp")

# find_package(pybind11 REQUIRED)
# find_package(Boost REQUIRED)
# find_package(QuantLib REQUIRED)
find_package(XAD REQUIRED)
# find_package(Atlas REQUIRED)

# pybind11_add_module(${PROJECT_NAME} MODULE ${SOURCES} ${INCLUDES})
# target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib Boost::boost XAD::xad Atlas::Atlas pybind11::module)

# target_include_directories(${PROJECT_NAME}
#   PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${Boost_INCLUDE_DIRS}
#     ${QuantLib_INCLUDE_DIRS}
#     ${XAD_INCLUDE_DIRS}
#     ${Atlas_INCLUDE_DIRS}
# )

string(TOUPPER "${CMAKE_BUILD_TYPE}" build_type_upper)
if(build_type_upper STREQUAL "RELEASE")
  get_target_property(XAD_LIB_LOCATION XAD::xad IMPORTED_LOCATION_RELEASE)
elseif(build_type_upper STREQUAL "RELWITHDEBINFO")
  get_target_property(XAD_LIB_LOCATION XAD::xad IMPORTED_LOCATION_RELWITHDEBINFO)
elseif(build_type_upper STREQUAL "DEBUG")
  get_target_property(XAD_LIB_LOCATION XAD::xad IMPORTED_LOCATION_DEBUG)
elseif(build_type_upper STREQUAL "MINSIZEREL")
  get_target_property(XAD_LIB_LOCATION XAD::xad IMPORTED_LOCATION_MINSIZEREL)
endif()

if (MSVC)
  get_filename_component(XAD_LIB_NAME ${XAD_LIB_LOCATION} NAME_WE)
else()
  set(XAD_LIB_NAME xad)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
               ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
               @ONLY)