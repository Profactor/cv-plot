# CvPlot - https://github.com/wpalfi/cv-plot
# Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

cmake_minimum_required (VERSION 3.9)
set(target CvPlot)
project (${target} CXX)

option(CVPLOT_HEADER_ONLY "Use as header only library" ON)

if(CVPLOT_HEADER_ONLY)
    set(empty_or_interface INTERFACE)
    set(public_or_interface INTERFACE)
else()
    set(empty_or_interface)
    set(public_or_interface PUBLIC)
endif()

add_library(${target} ${empty_or_interface})
target_sources(${target} ${public_or_interface}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/cvplot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/gui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/libdef.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/Axes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/Drawable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/DrawableDeque.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/makeAxes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/Projection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/RenderTarget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/core/Transformation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/Border.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/Group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/HorizontalGrid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/HorizontalLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/Image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/LineBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/LineType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/MarkerType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/Series.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/Title.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/VerticalGrid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/VerticalLine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/XAxis.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/XLabel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/YAxis.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/drawables/YLabel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/gui/MouseAdapter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/gui/MouseEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/gui/show.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/gui/Window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Axes.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Border.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/cvplot.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Drawable.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/DrawableDeque.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Group.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/HorizontalGrid.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/HorizontalLine.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Image.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/LineBase.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/makeAxes.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/MouseAdapter.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Series.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/Title.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/util.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/version.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/VerticalGrid.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/VerticalLine.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/XAxis.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/XLabel.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/YAxis.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/imp/YLabel.ipp
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/Internal/LinearProjection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/Internal/no_warning.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/Internal/Pimpl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/Internal/RawProjection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/CvPlot/Internal/util.h
)

add_library(${target}::${target} ALIAS ${target})
target_include_directories(${target} ${public_or_interface} ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(CVPLOT_HEADER_ONLY)
	target_compile_definitions(${target} INTERFACE CVPLOT_HEADER_ONLY)
else()
	target_sources(${target} PRIVATE src/cvplot.cpp)
	if(BUILD_SHARED_LIBS)
		target_compile_definitions(${target} PUBLIC CVPLOT_SHARED PRIVATE CVPLOT_EXPORTS)
	endif()
endif()

#OpenCV
if(NOT OpenCV_LIBS)
    find_package(OpenCV REQUIRED)
endif()
target_link_libraries(${target} ${public_or_interface} ${OpenCV_LIBS})

#Tests and Examples
option(CVPLOT_WITH_TESTS "Build with tests" ON)
option(CVPLOT_WITH_EXAMPLES "Build with examples" ON)

if(CVPLOT_WITH_TESTS OR CVPLOT_WITH_EXAMPLES)
    add_subdirectory(ext/catch2 catch2)
endif()

if(CVPLOT_WITH_TESTS)
    add_subdirectory(tests)
endif()

if(CVPLOT_WITH_EXAMPLES)
    add_subdirectory(examples)
endif()

