cmake_minimum_required(VERSION 3.16)
project(CvPlotExample)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::cvplot)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
