// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/HorizontalLine.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class HorizontalLine::Impl {
public:
    Impl(HorizontalLine &parent)
        :_parent(parent) {
    }
    HorizontalLine &_parent;
    double _pos = NAN;
    bool _boundingRectEnabled = false;

    void render(RenderTarget & renderTarget) {
        if (!std::isfinite(_pos)) {
            return;
        }
        cv::Mat3b& mat = renderTarget.innerMat();
        const int shift = 4;
        const int shiftScale = (1 << shift);
        auto p = renderTarget.project(cv::Point2d(0,_pos));
        int y = Internal::drawCast(p.y * shiftScale);
        cv::line(mat, cv::Point(0,y), cv::Point(mat.cols*shiftScale,y), _parent.getColor(), _parent.getLineWidth(), cv::LINE_AA, shift);
    }

    bool getBoundingRect(cv::Rect2d &rect) {
        if (!_boundingRectEnabled) {
            return false;
        }
        rect.x = NAN;
        rect.y = _pos;
        rect.width = NAN;
        rect.height = 0;
        return true;
    }
};

CVPLOT_DEFINE_FUN
HorizontalLine::~HorizontalLine() {
}

CVPLOT_DEFINE_FUN
HorizontalLine::HorizontalLine(double pos, const std::string &lineSpec)
    :LineBaseSub(lineSpec)
    ,impl(*this){
    setPos(pos);
    setLineSpec(lineSpec);
}

CVPLOT_DEFINE_FUN
void HorizontalLine::render(RenderTarget & renderTarget){
    impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool HorizontalLine::getBoundingRect(cv::Rect2d &rect) {
    return false;
}

CVPLOT_DEFINE_FUN
HorizontalLine & HorizontalLine::setPos(double pos){
    impl->_pos = pos;
    return *this;
}

CVPLOT_DEFINE_FUN
HorizontalLine & HorizontalLine::setBoundingRectEnabled(bool enabled) {
    impl->_boundingRectEnabled = enabled;
    return *this;
}

}
