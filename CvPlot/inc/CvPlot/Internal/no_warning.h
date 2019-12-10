// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/


#pragma once


/// Prevent dll interface warning on type
#ifdef WIN32
#define CVPLOT_NO_WARNING_DLL_INTERFACE(type, name) \
    __pragma(warning(push)) \
    __pragma(warning(disable : 4251)) \
    type name; \
    __pragma(warning(pop))
#else
#define CVPLOT_NO_WARNING_DLL_INTERFACE(type, name) \
    type name;
#endif
