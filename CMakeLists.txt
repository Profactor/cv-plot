# CvPlot - https://github.com/wpalfi/cv-plot
# Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

cmake_minimum_required (VERSION 3.9)
project (cv-plot)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#hack for static opencv from conan center
if(NOT CONAN_BIN_DIRS_OPENCV OR EXISTS "${CONAN_LIB_DIRS_OPENCV}/libopencv_core.a")
    set(OpenCV_STATIC ON)
    find_package(Eigen3 REQUIRED)
    
    find_package(protobuf REQUIRED)
    add_library(libprotobuf INTERFACE)
    target_link_libraries(libprotobuf INTERFACE protobuf::libprotobuf)
endif()

add_subdirectory(CvPlot)
