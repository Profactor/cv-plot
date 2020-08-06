// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/core/Transformation.h>
#include <opencv2/core.hpp>

namespace CvPlot {
//TODO namespace Internal{
struct RawProjection {
    cv::Point2d offset;
    double kx, ky;
    Transformation *transformation = nullptr;
    cv::Rect innerRect;

    cv::Point2d project(cv::Point2d point, bool withTransformation = true)const {
        if (transformation && withTransformation) {
            point = transformation->transform(point);
        }
        return cv::Point2d(offset.x + point.x*kx, offset.y + point.y*ky);
    }
    cv::Point2d unproject(cv::Point2d point, bool withTransformation = true)const {
        point = cv::Point2d((point.x - offset.x) / kx, (point.y - offset.y) / ky);
        if (transformation && withTransformation) {
            return transformation->untransform(point);
        }
        return point;
    }
};

}