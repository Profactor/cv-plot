// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/version.h>

namespace CvPlot {

CVPLOT_DEFINE_FUN
std::string version() {
    return CVPLOT_VERSION;
}

}

