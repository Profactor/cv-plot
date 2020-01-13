# CvPlot - https://github.com/Profactor/cv-plot
# Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

cmake_minimum_required (VERSION 3.9)
project (cv-plot)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#dont use find_package(OpenCV)
#(OpenCVConfig.cmake has some problems with conan package)
set(OpenCV_LIBS CONAN_PKG::opencv)

add_subdirectory(CvPlot)
