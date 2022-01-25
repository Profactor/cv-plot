# Integration
CvPlot requires OpenCV. It was tested with OpenCV 3.4 and 4.1, but it should run with any recent version on any platform. There are multiple ways to integrate CvPlot in your project:

## Header-only
This is the easiest way and should almost always work. Just be sure you have OpenCV available in the include path.
- Clone or download from https://github.com/Profactor/cv-plot/releases
- Add CvPlot/inc to your project's include path
- Define CVPLOT_HEADER_ONLY before including CvPlot

## Conan Package Manager
Just add `cvplot/1.2.2` to your conan requires. The [conan center package](https://conan.io/center/cvplot) is header only and uses [opencv](https://conan.io/center/opencv) from conan center. You can find an [example here](https://github.com/Profactor/cv-plot/tree/master/doc/conan). 

## CMake (advanced)
- The CMakeLists.txt file in the root folder expects that you run ´conan install´ before. 
- You may also build without Conan or add CvPlot to some project with add_subdirectory. In this case, use the CvPlot subdirectory, not the root CMakeLists.txt!
- Variables
   - CVPLOT_HEADER_ONLY [default ON]
   - CVPLOT_USE_CONAN [default ON] Turn off to use opencv from elsewhere
   - CVPLOT_WITH_TESTS [default ON]
   - CVPLOT_WITH_EXAMPLES [default ON]
   - [BUILD_SHARED_LIBS](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html)
