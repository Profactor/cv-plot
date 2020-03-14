![CvPlot](doc/img/contours.PNG)

[![Build status](https://github.com/Profactor/cv-plot/workflows/CI/badge.svg)](https://github.com/Profactor/cv-plot/actions)
[![Build status](https://ci.appveyor.com/api/projects/status/2bqhfcoh0q4w2gc8?svg=true)](https://ci.appveyor.com/project/Profactor/cv-plot)
[![Build Status](https://travis-ci.org/profactor/cv-plot.svg?branch=master)](https://travis-ci.org/profactor/cv-plot)

# Motivation
Yes, another C++ plotting library. Because CvPlot is

- Purely OpenCV based
- Highly adaptable and extendable
- [Fast](doc/img/benchmark.gif)
- [Easy to integrate](doc/tutorial.md#Integration)

CvPlot was developed at [PROFACTOR](https://www.profactor.at/) for realtime image plotting. It comes with some basic "Drawables", including Series, Image, Axis, Grid, Titles, etc. Drawables can easily be modified, replaced and extended, using standard opencv drawing functions. CvPlot comes with an interactive viewer, based on cv::imshow(). The viewer can easily be integrated into any C++ GUI framework (e.g. Qt/Qml in [CvPlotQt](https://github.com/Profactor/cv-plot-qt)).

# Warning
CvPlot is NOT and will never be a full featured plotting library. Many features are missing, but you can easily add them using [custom drawables](doc/tutorial.md#custom-drawables).

# Documentation
[Screenshots](doc/screenshots.md)

[Tutorial](doc/tutorial.md)



