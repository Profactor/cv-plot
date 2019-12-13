// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/core/Axes.h>
#include <opencv2/opencv.hpp>
#include <CvPlot/Internal/util.h>
#include <CvPlot/drawables/XLabel.h>
#include <CvPlot/drawables/YLabel.h>
#include <CvPlot/drawables/Title.h>
#include <CvPlot/drawables/HorizontalGrid.h>
#include <CvPlot/drawables/VerticalGrid.h>

namespace CvPlot {

class Axes::Impl {
public:
	Axes *_axes;
	int _leftMargin=80, _rightMargin=30, _topMargin=40, _bottomMargin=45;	
	std::pair<double, double> _xLim{ 0,1 };
	std::pair<double, double> _yLim{ 0,1 };
	bool _xLimAuto = true;
	bool _yLimAuto = true;
	bool _xTight = false;
	bool _yTight = false;
	bool _tightBox = false;
	bool _yReverse = false;
	bool _fixedAspectRatio = false;
    double _aspectRatio = 1;
	std::unique_ptr<Transformation> _transformation;
	bool _xLog = false;
	bool _yLog = false;

	Impl(Axes &axes) 
		:_axes(&axes){
	}
	
	RawProjection getRawProjection(cv::Size destinationSize) const{
		if(destinationSize.width <= _leftMargin+_rightMargin
			|| destinationSize.height <= _topMargin + _bottomMargin){
			return RawProjection();
		}
		cv::Rect innerRect = cv::Rect(_leftMargin, _topMargin, destinationSize.width - _leftMargin - _rightMargin, destinationSize.height - _topMargin - _bottomMargin);
		cv::Rect2d boundingRect = calcBoundingRect();
		cv::Rect2d viewport = getViewport(boundingRect);
		if (_fixedAspectRatio) {
			if (_tightBox && _xLimAuto && _yLimAuto) {
                double ratio = (viewport.height / viewport.width) * _aspectRatio;
				innerRect = Internal::fixRatio(innerRect, ratio, false);
			} else {
                double ratio = (innerRect.height / (double)innerRect.width) / _aspectRatio;
				viewport = Internal::fixRatio(viewport, ratio, true);
			}
		}
		RawProjection rawProjection;
		rawProjection.kx = innerRect.width / viewport.width;
		rawProjection.offset.x = -viewport.x * rawProjection.kx;
		rawProjection.ky = (innerRect.height / viewport.height) * (_yReverse ? 1 : -1);
		rawProjection.offset.y = -(viewport.y + (_yReverse ? 0 : viewport.height)) * rawProjection.ky;
		rawProjection.transformation = _transformation.get();
		rawProjection.innerRect = innerRect;
		return rawProjection;
	}

	void render(cv::Mat &mat, cv::Size destinationSize) const{
		RawProjection rawProjection = getRawProjection(destinationSize);
		mat.create(destinationSize, CV_8UC3);
		cv::Mat3b mat3b(mat);
		mat3b.setTo(cv::Vec3b::all(255));
		RenderTarget renderTarget(rawProjection, mat3b);
		for (const auto &drawable : _axes->drawables()) {
			drawable->render(renderTarget);
		}
	}

	cv::Rect2d calcBoundingRect() const{
		cv::Rect2d rect;
		if (!_axes->getBoundingRect(rect)) {
			rect = cv::Rect2d(0, 0, 1, 1);
		}
		Internal::normalize(rect);
		if (_transformation) {
			rect = _transformation->transformBoundingRect(rect);
			Internal::normalize(rect);
		}
		return rect;
	}

	std::pair<double, double> calcXLim(cv::Rect2d boundingRect) const {
		if (_xLimAuto) {
            if (!(std::isfinite(boundingRect.x) && std::isfinite(boundingRect.width))) {
                return {0,1};
            }
			if (boundingRect.width == 0) {
				return{ boundingRect.x - 1, boundingRect.x + 1};
			}
			else {
				return{ boundingRect.x, boundingRect.x + boundingRect.width };
			}
		}
		else {
			return _xLim;
		}
	}

	std::pair<double, double> calcYLim(cv::Rect2d boundingRect) const{
		if (_yLimAuto) {
            if (!(std::isfinite(boundingRect.y) && std::isfinite(boundingRect.height))) {
                return {0,1};
            }
			if (boundingRect.height == 0) {
				return{ boundingRect.y - 1, boundingRect.y + 1 };
			}
			else {
				return{ boundingRect.y, boundingRect.y + boundingRect.height };
			}
		}
		else {
			return _yLim;
		}
	}
	
	cv::Rect2d getViewport(cv::Rect2d boundingRect) const {
		auto xLim = calcXLim(boundingRect);
		auto yLim = calcYLim(boundingRect);
		cv::Rect2d viewport(xLim.first, yLim.first, xLim.second - xLim.first, yLim.second - yLim.first);
		const double ex = .1;
		if (!_xTight) {
			viewport.x -= viewport.width * ex / 2;
			viewport.width *= 1 + ex;
		}
		if (!_yTight) {
			viewport.y -= viewport.height * ex / 2;
			viewport.height *= 1 + ex;
		}
		return viewport;
	}

	void normalizeLims() {
		if (_xLim.first > _xLim.second) {
			std::swap(_xLim.first, _xLim.second);
		}
		if (_yLim.first > _yLim.second) {
			std::swap(_yLim.first, _yLim.second);
		}
	}

	void zoom(cv::Size size, cv::Point outerPos, double scalex, double scaley) {
		beginZoomOrPan();
		if (_fixedAspectRatio) {
            scalex = scaley = scalex * scaley;
		}
		RawProjection rawProjection = getRawProjection(size);
		if (!rawProjection.innerRect.area()) {
			return;
		}
		cv::Point2d topLeftPix0 = rawProjection.project(cv::Point2d(_xLim.first, _yLim.first), false);
		cv::Point2d bottomRightPix0 = rawProjection.project(cv::Point2d(_xLim.second, _yLim.second), false);
		cv::Point2d diag0 = bottomRightPix0 - topLeftPix0;
		cv::Point2d innerRectTopLeft(rawProjection.innerRect.x, rawProjection.innerRect.y);
		cv::Point2d pos0 = cv::Point2d(outerPos) - innerRectTopLeft -  topLeftPix0;
		cv::Point2d topLeftPix = topLeftPix0 + cv::Point2d(pos0.x*(1 - scalex), pos0.y*(1 - scaley));
		cv::Point2d bottomRightPix = topLeftPix + cv::Point2d(diag0.x*scalex, diag0.y*scaley);
		setLimsFromPoints(rawProjection, topLeftPix, bottomRightPix);
	}

	void pan(cv::Size size, cv::Point delta) {
		beginZoomOrPan();
		RawProjection rawProjection = getRawProjection(size);
		if (!rawProjection.innerRect.area()) {
			return;
		}
		cv::Point2d topLeftPix0 = rawProjection.project(cv::Point2d(_xLim.first, _yLim.first), false);
		cv::Point2d bottomRightPix0 = rawProjection.project(cv::Point2d(_xLim.second, _yLim.second), false);
		setLimsFromPoints(rawProjection, topLeftPix0 - cv::Point2d(delta), bottomRightPix0 - cv::Point2d(delta));
	}

	void beginZoomOrPan() {
		cv::Rect2d boundingRect = calcBoundingRect();
		if (_xLimAuto) {
			_xLim = calcXLim(boundingRect);
			_xLimAuto = false;
		}
		if (_yLimAuto) {
			_yLim = calcYLim(boundingRect);
			_yLimAuto = false;
		}
	}

	void setLimsFromPoints(const RawProjection &rawProjection, cv::Point2d topLeftPix, cv::Point2d bottomRightPix) {
		cv::Point2d topLeft = rawProjection.unproject(topLeftPix, false);
		cv::Point2d bottomRight = rawProjection.unproject(bottomRightPix, false);
		_xLim.first = topLeft.x;
		_xLim.second = bottomRight.x;
		_yLim.first = topLeft.y;
		_yLim.second = bottomRight.y;
		normalizeLims();
	}

	void setXLim(std::pair<double, double> xlim) {
		if (_transformation) {
			xlim = _transformation->transformXLim(xlim);
		}
		_xLim = xlim;
		_xLimAuto = false;
	}

	std::pair<double, double> getXLim() const {
		if (_transformation) {
			return _transformation->untransformXLim(_xLim);
		}
		return _xLim;
	}

	void setYLim(std::pair<double, double> ylim) {
		if (_transformation) {
			ylim = _transformation->transformYLim(ylim);
		}
		_yLim = ylim;
		_yLimAuto = false;
	}

	std::pair<double, double> getYLim() const {
		if (_transformation) {
			return _transformation->untransformYLim(_yLim);
		}
		return _yLim;
	}

	void setTransformation(std::unique_ptr<Transformation> transformation) {
		auto xlim = getXLim();
		auto ylim = getYLim();
		_transformation = std::move(transformation);
		if (!_xLimAuto) {
			setXLim(xlim);
		}
		if (!_yLimAuto) {
			setYLim(ylim);
		}
	}
	
	void setLogTransformation() {
		if (!_xLog && !_yLog) {
			setTransformation(nullptr);
		}
		if (!_xLog && _yLog) {
			setTransformation(std::unique_ptr<LinLogTransformation>(new LinLogTransformation()));
		}
		if (_xLog && !_yLog) {
			setTransformation(std::unique_ptr<LogLinTransformation>(new LogLinTransformation()));
		}
		if (_xLog && _yLog) {
			setTransformation(std::unique_ptr<LogLogTransformation>(new LogLogTransformation()));
		}
	}
};

CVPLOT_DEFINE_FUN
Axes::Axes()
	:impl(*this) {
}

CVPLOT_DEFINE_FUN
Axes::Axes(Axes &&a)
	:DrawableDeque(std::move(a))
	,impl(std::move(a.impl)){
	impl->_axes = this;
}

CVPLOT_DEFINE_FUN
Axes::~Axes(){
}

CVPLOT_DEFINE_FUN
Axes& Axes::operator=(Axes&& a) {
	DrawableDeque::operator=(std::move(a));
	impl = std::move(a.impl);
	impl->_axes = this;
	return *this;
}

CVPLOT_DEFINE_FUN
Projection Axes::getProjection(cv::Size size) const{
	return Projection(impl->getRawProjection(size));
}

CVPLOT_DEFINE_FUN cv::Mat3b Axes::render(int rows, int cols)const {
	cv::Mat3b mat;
	impl->render(mat, cv::Size(cols, rows));
	return mat;
}

CVPLOT_DEFINE_FUN
void Axes::render(const cv::Mat & mat) const{
    cv::Mat m(mat);
	impl->render(m, m.size());
}

CVPLOT_DEFINE_FUN
void Axes::render(cv::Mat & mat, cv::Size size)const{
	impl->render(mat, size);
}

CVPLOT_DEFINE_FUN
Axes& Axes::setMargins(int left, int right, int top, int bottom) {
	impl->_leftMargin = left;
	impl->_rightMargin = right;
	impl->_topMargin = top;
	impl->_bottomMargin = bottom;
	return *this;
}

CVPLOT_DEFINE_FUN
Axes & Axes::xLabel(const std::string & label) {
	findOrCreate<XLabel>().setLabel(label);
	return *this;
}

CVPLOT_DEFINE_FUN
Axes & Axes::yLabel(const std::string & label) {
	findOrCreate<YLabel>().setLabel(label).setYAxis(find<YAxis>());
	return *this;
}

CVPLOT_DEFINE_FUN
Axes & Axes::title(const std::string & title) {
	findOrCreate<Title>().setTitle(title);
	return *this;
}

CVPLOT_DEFINE_FUN
Axes & Axes::enableHorizontalGrid(bool enable) {
	auto &grid = findOrCreate<HorizontalGrid>().setEnabled(enable);
	if (enable) {
		grid.setYAxis(find<YAxis>());
	}
	return *this;
}

CVPLOT_DEFINE_FUN
Axes & Axes::enableVerticalGrid(bool enable) {
	auto &grid = findOrCreate<VerticalGrid>().setEnabled(enable);
	if (enable) {
		grid.setXAxis(find<XAxis>());
	}
	return *this;
}

CVPLOT_DEFINE_FUN
void Axes::zoom(cv::Size size, cv::Point outerPos, double scalex, double scaley) {
	impl->zoom(size, outerPos, scalex, scaley);
}

CVPLOT_DEFINE_FUN
void Axes::pan(cv::Size size, cv::Point delta) {
	impl->pan(size, delta);
}

CVPLOT_DEFINE_FUN
Axes& Axes::setXLim(std::pair<double, double> xlim) {
	impl->setXLim(xlim);
	return *this;
}

CVPLOT_DEFINE_FUN
std::pair<double, double> Axes::getXLim() const {
	return impl->getXLim();
}

CVPLOT_DEFINE_FUN
Axes& Axes::setXLimAuto(bool xLimAuto) {
	impl->_xLimAuto = xLimAuto;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getXLimAuto() const {
	return impl->_xLimAuto;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setYLim(std::pair<double, double> ylim) {
	impl->setYLim(ylim);
	return *this;
}

CVPLOT_DEFINE_FUN
std::pair<double, double> Axes::getYLim() const {
	return impl->getYLim();
}

CVPLOT_DEFINE_FUN
Axes& Axes::setYLimAuto(bool yLimAuto) {
	impl->_yLimAuto = yLimAuto;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getYLimAuto() const {
	return impl->_yLimAuto;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setYReverse(bool reverse) {
	impl->_yReverse = reverse;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getYReverse() const {
	return impl->_yReverse;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setFixedAspectRatio(bool fixed) {
	impl->_fixedAspectRatio = fixed;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getFixedAspectRatio() const {
	return impl->_fixedAspectRatio;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setAspectRatio(double aspectRatio) {
    impl->_aspectRatio = aspectRatio;
    return *this;
}

CVPLOT_DEFINE_FUN
double Axes::getAspectRatio()const {
    return impl->_aspectRatio;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setXTight(bool tight) {
	impl->_xTight = tight;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getXTight()const {
	return impl->_xTight;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setYTight(bool tight) {
	impl->_yTight = tight;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getYTight()const {
	return impl->_yTight;
}

CVPLOT_DEFINE_FUN
Axes & Axes::setTightBox(bool tight) {
	impl->_tightBox = tight;
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getTightBox() const {
	return impl->_tightBox;
}

CVPLOT_DEFINE_FUN
Axes &Axes::setTransformation(std::unique_ptr<Transformation> transformation) {
	impl->setTransformation(std::move(transformation));
	return *this;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setXLog(bool log) {
	impl->_xLog = log;
	impl->setLogTransformation();
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getXLog() const {
	return impl->_xLog;
}

CVPLOT_DEFINE_FUN
Axes& Axes::setYLog(bool log) {
	impl->_yLog = log;
	impl->setLogTransformation();
	return *this;
}

CVPLOT_DEFINE_FUN
bool Axes::getYLog() const {
	return impl->_yLog;
}

}
