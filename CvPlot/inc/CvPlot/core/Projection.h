// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/internal/RawProjection.h>
#include <opencv2/core.hpp>

namespace CvPlot {

class Projection {
public:
	Projection(RawProjection rawProjection)
		: _rawProjection(rawProjection){
	}
	cv::Point2d project(cv::Point2d point)const {
		return _rawProjection.project(point);
	}
	cv::Point2d unproject(cv::Point2d point)const {
		return _rawProjection.unproject(point);
	}
	cv::Rect innerRect()const {
		return _rawProjection.innerRect;
	}
	cv::Point2d outerToInner(cv::Point2d outer) const{
		return cv::Point2d(outer.x - _rawProjection.innerRect.x, outer.y - _rawProjection.innerRect.y);
	}
	cv::Point2d innerToOuter(cv::Point2d inner) const {
		return cv::Point2d(inner.x + _rawProjection.innerRect.x, inner.y + _rawProjection.innerRect.y);
	}
private:
	RawProjection _rawProjection;
};

}