// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <string>

#define CVPLOT_VERSION "1.0"

namespace CvPlot {

CVPLOT_LIBRARY_INTERFACE std::string version();

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/version.ipp>
#endif