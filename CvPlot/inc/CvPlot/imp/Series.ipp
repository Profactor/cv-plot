// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/Series.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>
#include <limits>

namespace CvPlot {

class Series::Impl {
public:
    Impl(Series &parent)
        :_parent(parent) {
    }
    Series &_parent;
    std::vector<double> _x,_internalX,_y;
    MarkerType _markerType = MarkerType::None;
    int _markerSize = 10;
    cv::Rect2d _boundingRect;

    bool xyValid(cv::InputArray a) {
        cv::Mat m = a.getMat();
        return m.empty() || (m.channels() == 1 && (m.rows == 1 || m.cols == 1));
    }
    bool pointsValid(cv::InputArray a) {
        cv::Mat p = a.getMat();
        return p.empty() 
            || (p.channels() == 2 && (p.cols == 1 || p.rows == 1))
            || (p.channels() == 1 && (p.rows == 2));
    }
    void update() {
        if (_y.empty()) {
            _internalX.clear();
            return;
        }
        if (_x.empty()) {
            _internalX = Internal::makeX(_y);
        } else {
            if (_x.size() != _y.size()) {
                _internalX.clear();
                return;
            }
            _internalX = _x;
        }
        double minx, maxx, miny, maxy;
        cv::minMaxIdx(_internalX, &minx, &maxx);
        cv::minMaxIdx(_y, &miny, &maxy);
        _boundingRect = cv::Rect2d(minx, miny, maxx - minx, maxy - miny);
    }
    std::vector<std::vector<cv::Point>> getShiftedPoints(const std::vector<cv::Point2d> &points, int shiftScale) {
        std::vector<std::vector<cv::Point>> groups;
        std::vector<cv::Point> *group = nullptr;
        for (const auto &p : points) {
            if (std::isfinite(p.x) && std::isfinite(p.y)) {
                if (!group) {
                    groups.push_back({});
                    group = &groups.back();
                    group->reserve(points.size());
                }
                group->emplace_back(Internal::drawCast(p.x * shiftScale), Internal::drawCast(p.y * shiftScale));
            } else {
                group = nullptr;
            }
        }
        return groups;
    }

    void render(RenderTarget & renderTarget) {
        if (_internalX.empty()) {
            return;
        }
        const cv::Scalar color = _parent.getColor();
        const int lineWidth = _parent.getLineWidth();
        if (_markerType == MarkerType::None && lineWidth == 0) {
            return;
        }
        cv::Mat3b& mat = renderTarget.innerMat();
        const int shift = 3;
        const int shiftScale = (1 << shift);
        std::vector<cv::Point2d> points;
        points.reserve(_internalX.size());
        for (size_t i = 0; i < _internalX.size(); i++) {
            points.push_back(renderTarget.project(cv::Point2d(_internalX[i], _y[i])));
        }
        std::vector<std::vector<cv::Point>> shiftedPoints;
        if (_parent.getLineType() == LineType::Solid) {
            if (shiftedPoints.empty()) {
                shiftedPoints = getShiftedPoints(points, shiftScale);
            }
            cv::polylines(mat, shiftedPoints, false, color, lineWidth, cv::LINE_AA, shift);
        }
        if (_markerType == MarkerType::Circle) {
            if (shiftedPoints.empty()) {
                shiftedPoints = getShiftedPoints(points, shiftScale);
            }
            const int radius = (_markerSize * shiftScale) / 2;
            for (const auto &group: shiftedPoints ) {
                for (const auto &point : group) {
                    cv::circle(mat, point, radius, color, lineWidth, cv::LINE_AA, shift);
                }
            }
        }
        if (_markerType == MarkerType::Point) {
            cv::Vec3b colorv3((unsigned char)color.val[0], (unsigned char)color.val[1], (unsigned char)color.val[2]);
            for (size_t i = 0; i < points.size(); i++) {
                const auto p = (cv::Point)points[i];
                if (p.x >= 0 && p.x < mat.cols && p.y >= 0 && p.y < mat.rows) {
                    mat(p.y, p.x) = colorv3;
                }
            }
        }
    }
    std::vector<cv::Point2d> getPoints() {
        std::vector<cv::Point2d> points;
        points.reserve(_internalX.size());
        for (size_t i = 0; i < _internalX.size(); i++) {
            points.emplace_back(_internalX[i], _y[i]);
        }
        return points;
    }
};

CVPLOT_DEFINE_FUN
Series::~Series() {
}

CVPLOT_DEFINE_FUN
Series::Series(const std::string & lineSpec)
    : Series(cv::Mat(), lineSpec) {
}

CVPLOT_DEFINE_FUN
Series::Series(cv::InputArray data, const std::string &lineSpec)
    : LineBaseSub(lineSpec)
    , impl(*this) {
    cv::Mat d = data.getMat();
    if (impl->xyValid(data)){
        setY(data);
    }else if(impl->pointsValid(data)){
        setPoints(data);
    }else {
        throw std::invalid_argument("invalid data in Series constructor. See Series.h for supported types");
    }
    setLineSpec(lineSpec);
}

CVPLOT_DEFINE_FUN
Series::Series(cv::InputArray x, cv::InputArray y, const std::string &lineSpec)
    :LineBaseSub(lineSpec)
    ,impl(*this){
    if (!impl->xyValid(x)) {
        throw std::invalid_argument("invalid x in Series constructor. See Series.h for supported types");
    }
    if (!impl->xyValid(y)) {
        throw std::invalid_argument("invalid y in Series constructor. See Series.h for supported types");
    }
    setX(x);
    setY(y);
    setLineSpec(lineSpec);
}

CVPLOT_DEFINE_FUN
void Series::render(RenderTarget & renderTarget){
    impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
Series& Series::setLineSpec(const std::string &lineSpec) {
    LineBase::setLineSpec(lineSpec);
    if (lineSpec.find_first_of('o') != std::string::npos) {
        setMarkerType(MarkerType::Circle);
    }else if (lineSpec.find_first_of('.') != std::string::npos) {
        setMarkerType(MarkerType::Point);
    }else{
        setMarkerType(MarkerType::None);
    }
    return *this;
}

CVPLOT_DEFINE_FUN
Series& Series::setMarkerType(MarkerType markerType){
    impl->_markerType = markerType;
    return *this;
}

CVPLOT_DEFINE_FUN
MarkerType Series::getMarkerType(){
    return impl->_markerType;
}

CVPLOT_DEFINE_FUN
Series& Series::setMarkerSize(int markerSize) {
    impl->_markerSize = markerSize;
    return *this;
}

CVPLOT_DEFINE_FUN
int Series::getMarkerSize() {
    return impl->_markerSize;
}

CVPLOT_DEFINE_FUN
bool Series::getBoundingRect(cv::Rect2d &rect) {
    if (impl->_internalX.empty()) {
        return false;
    }
    rect = impl->_boundingRect;
    return true;
}

CVPLOT_DEFINE_FUN
Series & Series::setX(cv::InputArray x) {
    if (!impl->xyValid(x)) {
        throw std::invalid_argument("invalid x in Series::setX(). See Series.h for supported types");
    }
    impl->_x = Internal::toVector<double>(x);
    impl->update();
    return *this;
}

CVPLOT_DEFINE_FUN
std::vector<double> Series::getX() {
    return impl->_x;
}

CVPLOT_DEFINE_FUN
Series & Series::setY(cv::InputArray y) {
    if (!impl->xyValid(y)) {
        throw std::invalid_argument("invalid y in Series::setY(). See Series.h for supported types");
    }
    impl->_y = Internal::toVector<double>(y);
    impl->update();
    return *this;
}

CVPLOT_DEFINE_FUN
std::vector<double> Series::getY() {
    return impl->_y;
}

CVPLOT_DEFINE_FUN
Series & Series::setPoints(cv::InputArray points){
    if (!impl->pointsValid(points)) {
        throw std::invalid_argument("invalid points in Series::setPoints(). See Series.h for supported types");
    }
    cv::Mat p = points.getMat();
    if (p.empty()) {
        impl->_x.clear();
        impl->_y.clear();
    }else if (p.channels() == 2) {
        cv::Mat channels[2];
        cv::split(points.getMat(), channels);
        impl->_x = Internal::toVector<double>(channels[0]);
        impl->_y = Internal::toVector<double>(channels[1]);
    } else {
        impl->_x = Internal::toVector<double>(p.row(0));
        impl->_y = Internal::toVector<double>(p.row(1));
    }
    impl->update();
    return *this;
}

CVPLOT_DEFINE_FUN
std::vector<cv::Point2d> Series::getPoints(){
    return impl->getPoints();
}

}
