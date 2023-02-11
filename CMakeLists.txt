cmake_minimum_required(VERSION 3.0.0)
project(Atlas VERSION 0.1.0)

include(GNUInstallDirs) # despues de definir el proyecto
set(CMAKE_CXX_STANDARD 20)
set(BUILD_PYTHON OFF)
set(BUILD_TESTS ON)
set(BUILD_BENCHMARK OFF)
# target is curvemanager, and use include a src folders

file(
  GLOB
  SOURCES
  "src/*.cpp"
  "src/data/*.cpp"
  "src/cashflows/*.cpp"
  "src/instruments/*.cpp"
  "src/instruments/fixedrate/*.cpp"
  "src/visitors/*.cpp"
  "src/models/*.cpp"
  "src/instruments/floatingrate/*.cpp"
  "src/multithreading/*.cpp")

file(
  GLOB
  INCLUDES
  "include/atlas/*.hpp"
  "include/atlas/curves/*.hpp"
  "include/atlas/data/*.hpp"
  "include/atlas/instruments/*.hpp"
  "include/atlas/instruments/fixedrate/*.hpp"
  "include/atlas/instruments/floatingrate/*.hpp"
  "include/atlas/cashflows/*.hpp"
  "include/atlas/models/*.hpp"
  "include/atlas/multithreading/*.hpp"
  "include/atlas/visitors/*.hpp"
  "include/atlas/others/*.hpp")

if(MSVC)
  add_library(${PROJECT_NAME} STATIC ${INCLUDES} ${SOURCES})
else()
  add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES})
endif(MSVC)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)


target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
               $<INSTALL_INTERFACE:include>)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

target_link_libraries(${PROJECT_NAME}  PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME}  PUBLIC Boost::boost)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake)

set(CONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
    CACHE INTERNAL "")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")

# export targets to be used with find_package
install(
  TARGETS ${PROJECT_NAME} 
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# exports target config file to be used with find_package
install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CONFIG_INSTALL_DIR})

# exports config file to be used with find_package
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CONFIG_INSTALL_DIR})

# exports headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include
        DESTINATION ${CMAKE_INSTALL_PREFIX})

if(BUILD_PYTHON)
  add_subdirectory(python)
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(BUILD_BENCHMARK)
  add_subdirectory(benchmark)
endif()
