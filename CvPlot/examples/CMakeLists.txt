set(target CvPlotExamples)
project(${target} CXX)
cmake_minimum_required (VERSION 3.9)
add_executable(${target}
    mandelbrot.h
    examples_core.cpp
    examples_gui.cpp
    main.cpp
)
target_link_libraries(${target} CvPlot::CvPlot)

#catch2
target_link_libraries(${target} Catch2::Catch2)

#data
function(copyfile file)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_CURRENT_SOURCE_DIR}/${file} 
            $<TARGET_FILE_DIR:${target}>/${file}
        COMMENT "copy ${file}..."
    )
endfunction()
copyfile(exampledata/Lenna.jpg)

#working directory
get_target_property(outdir ${target} RUNTIME_OUTPUT_DIRECTORY)
set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${outdir})
