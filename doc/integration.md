# Integration
CvPlot requires OpenCV. It was tested with OpenCV 3.4 and 4.1, but it should run with any recent version on any platform. There are multiple ways to integrate CvPlot in your project:

## Header-only
This is the easiest way and should almost always work. Just be sure you have OpenCV available in the include path.
- Clone or download from https://github.com/Profactor/cv-plot/releases
- Add CvPlot/inc to your project's include path

## Conan Package Manager
[Conan](https://conan.io/) packages are available here: https://bintray.com/profactor/conan/CvPlot%3Aprofactor-public
- Run this from command line: conan remote add profactor https://api.bintray.com/conan/profactor/conan 
- Add this requirement to your conanfile: CvPlot/1.2@profactor-public/stable
- Options:
   - CvPlot:header_only [default False]
   - CvPlot:shared [default False] 
- Conan will automatically install OpenCV from the conan center. You may [override](https://docs.conan.io/en/latest/using_packages/conanfile_txt.html#overriding-requirements) the OpenCV version in your conanfile. (Warning: Overriding with precompiled library may be dangerous, depending on the binary compatibility of OpenCV.)
- If you cannot find a matching binary package, use header_only=True.

## CMake (advanced)
- The CMakeLists.txt file in the root folder expects that you run ´conan install´ before. 
- You may also build without Conan or add CvPlot to some project with add_subdirectory. In this case, use the CvPlot subdirectory, not the root CMakeLists.txt!
- Variables
   - CVPLOT_HEADER_ONLY [default ON]
   - CVPLOT_USE_CONAN [default ON] Turn off to use opencv from elsewhere
   - CVPLOT_WITH_TESTS [default ON]
   - CVPLOT_WITH_EXAMPLES [default ON]
   - [BUILD_SHARED_LIBS](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html)
