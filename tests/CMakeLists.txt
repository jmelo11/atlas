cmake_minimum_required(VERSION 3.10)

project(
  AtlasTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)

file(GLOB SOURCES "*.cpp" "coupons/*.cpp" "instruments/*.cpp" "visitors/*.cpp" "others/*.cpp" "rates/*.cpp")

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

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)
find_package(autodiff REQUIRED)
find_package(Eigen3 REQUIRED)

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

target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
target_link_libraries(${PROJECT_NAME} PUBLIC autodiff::autodiff)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)