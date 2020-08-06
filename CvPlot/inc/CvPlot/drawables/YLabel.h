// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/core/Drawable.h>
#include <CvPlot/drawables/YAxis.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE YLabel : public Drawable {
public:
    YLabel(const std::string &label="", YAxis* yAxis=nullptr);
    ~YLabel();
    void render(RenderTarget &renderTarget)override;
    bool getBoundingRect(cv::Rect2d &rect)override;
    YLabel& setLabel(const std::string &label);
    std::string getLabel();
    YLabel& setYAxis(YAxis* yAxis);
private:
    class Impl;
    CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/YLabel.ipp>
#endif
