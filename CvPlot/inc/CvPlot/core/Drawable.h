// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/core/RenderTarget.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/Internal/Pimpl.h>
#include <string>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Drawable {
public:
    Drawable(std::string name = "");
    Drawable(Drawable &&a);
    virtual ~Drawable();
	virtual void render(RenderTarget &renderTarget);
	virtual bool getBoundingRect(cv::Rect2d &rect);
    Drawable& setName(std::string name);
    const std::string& getName();
private:
    class Impl;
    CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Drawable.ipp>
#endif