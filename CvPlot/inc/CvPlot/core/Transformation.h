// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>

namespace CvPlot {

class Transformation{
public:
    virtual ~Transformation(){}
    virtual cv::Point2d transform(const cv::Point2d &point)const = 0;
    virtual cv::Point2d untransform(const cv::Point2d &point)const = 0;
    virtual cv::Rect2d transformBoundingRect(const cv::Rect2d &r)const {
        return cv::Rect2d(transform(cv::Point2d(r.x, r.y)), transform(cv::Point2d(r.x + r.width, r.y + r.height)));
    }
    virtual std::pair<double, double> transformXLim(std::pair<double, double> xlim)const {
        return std::make_pair(transform(cv::Point2d(xlim.first, 0)).x, transform(cv::Point2d(xlim.second, 0)).x);
    }
    virtual std::pair<double, double> transformYLim(std::pair<double, double> ylim)const {
        return std::make_pair(transform(cv::Point2d(0, ylim.first)).y, transform(cv::Point2d(0, ylim.second)).y);
    }
    virtual std::pair<double, double> untransformXLim(std::pair<double, double> xlim)const {
        return std::make_pair(untransform(cv::Point2d(xlim.first, 0)).x, untransform(cv::Point2d(xlim.second, 0)).x);
    }
    virtual std::pair<double, double> untransformYLim(std::pair<double, double> ylim)const {
        return std::make_pair(untransform(cv::Point2d(0, ylim.first)).y, untransform(cv::Point2d(0, ylim.second)).y);
    }
};

class LogLogTransformation : public Transformation {
public:
    cv::Point2d transform(const cv::Point2d &point)const override {
        return cv::Point2d(std::log(point.x), std::log(point.y));
    }
    cv::Point2d untransform(const cv::Point2d &point)const override {
        return cv::Point2d(std::exp(point.x), std::exp(point.y));
    }
    std::pair<double, double> transformXLim(std::pair<double, double> xlim)const override {
        return LogLogTransformation::transformLogLim(xlim);
    }
    std::pair<double, double> transformYLim(std::pair<double, double> ylim)const override {
        return LogLogTransformation::transformLogLim(ylim);
    }
    static std::pair<double, double> transformLogLim(std::pair<double, double> lim) {
        if (lim.first <= 0) {
            lim.first = 1e-5;
        }
        if (lim.second <= lim.first) {
            lim.second = lim.first * 10;
        }
        lim.first = std::log(lim.first);
        lim.second = std::log(lim.second);
        return lim;
    }
};

class LinLogTransformation : public Transformation {
public:
    cv::Point2d transform(const cv::Point2d &point)const override {
        return cv::Point2d(point.x, std::log(point.y));
    }
    cv::Point2d untransform(const cv::Point2d &point)const override {
        return cv::Point2d(point.x, std::exp(point.y));
    }
    std::pair<double, double> transformYLim(std::pair<double, double> ylim)const override{
        return LogLogTransformation::transformLogLim(ylim);
    }
};

class LogLinTransformation : public Transformation {
public:
    cv::Point2d transform(const cv::Point2d &point)const override {
        return cv::Point2d(std::log(point.x), point.y);
    }
    cv::Point2d untransform(const cv::Point2d &point)const override {
        return cv::Point2d(std::exp(point.x), point.y);
    }
    std::pair<double, double> transformXLim(std::pair<double, double> xlim)const override {
        return LogLogTransformation::transformLogLim(xlim);
    }
};

}