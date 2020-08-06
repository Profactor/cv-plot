// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/drawables/LineBase.h>
#include <CvPlot/drawables/MarkerType.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Series : public LineBaseSub<Series> {
public:
    Series(const std::string &lineSpec = "-");
    
    //data: see setY() and setPoints()
    Series(cv::InputArray data, const std::string &lineSpec = "-");

    //x,y: see setX()
    Series(cv::InputArray x, cv::InputArray y, const std::string &lineSpec="-");

    ~Series();
    void render(RenderTarget &renderTarget)override;
    bool getBoundingRect(cv::Rect2d &rect)override;

    //x: 
    // - std::vector<T> of scalar
    // - cv::Mat 1-channel (1xN or Nx1)
    Series& setX(cv::InputArray x);
    std::vector<double> getX();

    //y: see setX() 
    Series& setY(cv::InputArray y);
    std::vector<double> getY();
    
    //points:
    // - std::vector<cv::Vec<T, 2>>
    // - std::vector<cv::Point_<T>>
    // - cv::Mat 2-channel (1xN or Nx1)
    // - cv::Mat 1-channel (2xN)
    Series& setPoints(cv::InputArray points);
    std::vector<cv::Point2d> getPoints();
    
    Series& setLineSpec(const std::string &lineSpec);
    Series& setMarkerType(MarkerType markerType);
    MarkerType getMarkerType();
    Series& setMarkerSize(int markerSize);
    int getMarkerSize();
private:
    class Impl;
    CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}
#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Series.ipp>
#endif
