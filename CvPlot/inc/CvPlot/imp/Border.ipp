// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/Border.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class Border::Impl {
public:
    cv::Scalar _color = cv::Scalar(0, 0, 0);
};

CVPLOT_DEFINE_FUN
Border::~Border() {
}

CVPLOT_DEFINE_FUN
Border::Border(){
}

CVPLOT_DEFINE_FUN
void Border::render(RenderTarget & renderTarget){
    cv::Rect borderRect = renderTarget.innerRect();        
    if (!borderRect.area()) {
        return;
    }
    borderRect.x--;
    borderRect.y--;
    borderRect.height += 2;
    borderRect.width += 2;
    cv::rectangle(renderTarget.outerMat(), borderRect, impl->_color);
}

CVPLOT_DEFINE_FUN
bool Border::getBoundingRect(cv::Rect2d &rect) {
    return false;
}

}
