// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <vector>
#include <opencv2/opencv.hpp>

namespace CvPlot {
namespace Internal {

int digits(double value);
std::string format(double value, bool exponentional = false);
std::vector<double> calcTicksLinear(double x0, double x1, int estimatedTickCount);
std::vector<double> calcTicksLog(double x0, double x1, int estimatedTickCount);
std::vector<double> makeX(const std::vector<double>& y);
int drawCast(double value);
cv::Rect boundingRect(cv::Rect2d src);
void paint(const cv::Mat &src, cv::Mat &dst, const cv::Point &pos);
void paint(const cv::Mat3b &src, cv::Mat3b &dst, const cv::Rect2d &pos, int interpolation, const cv::Mat &data);

template<typename T>
void normalize(cv::Rect_<T> &rect) {
    if (rect.width < 0) {
        rect.x += rect.width;
        rect.width *= -1;
    }
    if (rect.height < 0) {
        rect.y += rect.height;
        rect.height *= -1;
    }
}

template<typename T>
std::vector<T> toVector(cv::InputArray inputArray) {
    cv::Mat_<T> mat(inputArray.getMat());
    if (!mat.isContinuous()) {
        mat = mat.clone();
    }
    const T* data = reinterpret_cast<T*>(mat.data);
    return std::vector<T>(data, data + mat.total());
}

template<typename T>
cv::Rect_<T> fixRatio(cv::Rect_<T> rect, double ratio, bool extend) {
    normalize(rect);
    double curRatio = rect.height / (double)rect.width;
    cv::Rect_<T> fixed = rect;
    if ((curRatio < ratio) == extend) {
        fixed.height = static_cast<T>(rect.width * ratio);
        fixed.y -= (fixed.height - rect.height) / 2;
    }
    else {
        fixed.width = static_cast<T>(rect.height / ratio);
        fixed.x -= (fixed.width - rect.width) / 2;
    }
    return fixed;
}

}
}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/util.ipp>
#endif

