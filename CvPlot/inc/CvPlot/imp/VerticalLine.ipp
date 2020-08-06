// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/VerticalLine.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class VerticalLine::Impl {
public:
    Impl(VerticalLine &parent)
        :_parent(parent) {
    }
    VerticalLine &_parent;
    double _pos = NAN;
    bool _boundingRectEnabled = false;

    void render(RenderTarget & renderTarget) {
        if (!std::isfinite(_pos)) {
            return;
        }
        cv::Mat3b& mat = renderTarget.innerMat();
        const int shift = 4;
        const int shiftScale = (1 << shift);
        auto p = renderTarget.project(cv::Point2d(_pos,0));
        int x = Internal::drawCast(p.x * shiftScale);
        cv::line(mat, cv::Point(x,0), cv::Point(x, mat.rows*shiftScale), _parent.getColor(), _parent.getLineWidth(), cv::LINE_AA, shift);
    }

    bool getBoundingRect(cv::Rect2d &rect) {
        if (!_boundingRectEnabled) {
            return false;
        }
        rect.x = _pos;
        rect.y = NAN;
        rect.width = 0;
        rect.height = NAN;
        return true;
    }
};

CVPLOT_DEFINE_FUN
VerticalLine::~VerticalLine() {
}

CVPLOT_DEFINE_FUN
VerticalLine::VerticalLine(double pos, const std::string &lineSpec)
    :LineBaseSub(lineSpec)
    ,impl(*this){
    setPos(pos);
    setLineSpec(lineSpec);
}

CVPLOT_DEFINE_FUN
void VerticalLine::render(RenderTarget & renderTarget){
    impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool VerticalLine::getBoundingRect(cv::Rect2d &rect) {
    return impl->getBoundingRect(rect);
}

CVPLOT_DEFINE_FUN
VerticalLine & VerticalLine::setPos(double pos){
    impl->_pos = pos;
    return *this;
}

CVPLOT_DEFINE_FUN
VerticalLine & VerticalLine::setBoundingRectEnabled(bool enabled) {
    impl->_boundingRectEnabled = enabled;
    return *this;
}

}
