// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/XLabel.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class XLabel::Impl {
public:
    std::string _label;
    const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
    const double _fontScale = .4;
    const int _fontThickness = 1;
    cv::Scalar _color = cv::Scalar(0, 0, 0);

    int getTextWidth(const std::string &text) {
        int baseline;
        return cv::getTextSize(text, _fontFace, _fontScale, _fontThickness, &baseline).width;
    }
    void render(RenderTarget & renderTarget) {
        cv::Mat3b &outerMat = renderTarget.outerMat();
        auto innerRect = renderTarget.innerRect();
        if (!innerRect.area() || _label.empty()) {
            return;
        }
        int bottom = innerRect.y + innerRect.height;
        int xcenter = innerRect.x + innerRect.width / 2;
        int w = getTextWidth(_label);
        cv::Point labelPos(xcenter - w / 2, bottom + 35);
        cv::putText(outerMat, _label, labelPos, _fontFace, _fontScale, _color, _fontThickness, cv::LINE_AA);
    }
};

CVPLOT_DEFINE_FUN
XLabel::~XLabel() {
}

CVPLOT_DEFINE_FUN
XLabel::XLabel(const std::string &label){
    setLabel(label);
}

CVPLOT_DEFINE_FUN
void XLabel::render(RenderTarget & renderTarget){
    impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool XLabel::getBoundingRect(cv::Rect2d &rect) {
    return false;
}

CVPLOT_DEFINE_FUN
XLabel & XLabel::setLabel(const std::string & label){
    impl->_label = label;
    return *this;
}

CVPLOT_DEFINE_FUN
std::string XLabel::getLabel(){
    return impl->_label;
}
}
