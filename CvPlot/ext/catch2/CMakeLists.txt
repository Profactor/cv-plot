# CvPlot - https://github.com/wpalfi/cv-plot
# Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

cmake_minimum_required (VERSION 3.9)
project (Catch2 CXX)
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
add_library(Catch2::Catch2 ALIAS Catch2)
