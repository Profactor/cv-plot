// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/core/Projection.h>

namespace CvPlot {

class RenderTarget :public Projection{
public:
    RenderTarget(RawProjection rawProjection, cv::Mat3b &outerMat)
        : Projection(rawProjection)
        ,_outerMat(outerMat)
        ,_innerMat(outerMat(rawProjection.innerRect)){
    }
    cv::Mat3b& innerMat(){
        return _innerMat;
    }
    cv::Mat3b& outerMat() {
        return _outerMat;
    }
private:
    cv::Mat3b _outerMat;
    cv::Mat3b _innerMat;
};

}
