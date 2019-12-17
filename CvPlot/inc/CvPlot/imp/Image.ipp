// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/Image.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

namespace Imp{

CVPLOT_DEFINE_FUN
cv::Mat1b toMat1b(const cv::Mat& mat) {
    bool floating = mat.depth() == CV_32F || mat.depth() == CV_64F;
    cv::Mat mask = floating ? (mat == mat) : cv::Mat();
    double minVal, maxVal;
    cv::minMaxLoc(mat, &minVal, &maxVal, nullptr, nullptr, mask);
	const double alpha = 255.0 / (maxVal - minVal);
	const double beta = -minVal * alpha;
    cv::Mat1b mat1b;
	mat.convertTo(mat1b, mat1b.type(), alpha, beta);
    return mat1b;
}

CVPLOT_DEFINE_FUN
cv::Mat3b toMat3b(const cv::Mat& mat, int code) {
    cv::Mat3b mat3b;
    if (!mat.empty()) {
        cv::cvtColor(mat, mat3b, code);
    }
    return mat3b;
}

CVPLOT_DEFINE_FUN
cv::Mat3b toBgr(const cv::Mat& mat, cv::Scalar nanColor) {
	switch (mat.type()) {
	case CV_8UC3:
		return mat;
	case CV_8UC4:
		return toMat3b(mat, cv::COLOR_BGRA2BGR);
    case CV_16S:
    case CV_16U:
    case CV_32S:
        return toMat3b(toMat1b(mat), cv::COLOR_GRAY2BGR);
	case CV_32F:
    case CV_64F: {
        auto mat3b = toMat3b(toMat1b(mat), cv::COLOR_GRAY2BGR);
        if (nanColor != cv::Scalar()) {
            mat3b.setTo(nanColor, mat != mat);
        }
        return mat3b;
    }
	case CV_8UC1:
        return toMat3b(mat, cv::COLOR_GRAY2BGR);
	default:
		throw std::runtime_error("Image: mat type not supported");
	}
}

}

class Image::Impl {
public:
    cv::Mat _mat;
    cv::Mat3b _matBgr;
	cv::Rect2d _position;
	bool _autoPosition = true;
	int _interpolation = cv::INTER_AREA;
    cv::Scalar _nanColor;

	cv::Mat3b _flipped;
	bool _flipVert = false;
	bool _flipHorz = false;

	void updateFlipped() {
		if (_mat.empty()) {
			return;
		}
		if (_flipVert || _flipHorz) {
			int code = (_flipVert&&_flipHorz) ? -1 : (_flipVert ? 0 : +1);
			cv::flip(_matBgr, _flipped, code);
		}else {
			_flipped = _matBgr;
		}
	}
	void render(RenderTarget & renderTarget) {
		if (_mat.empty()) {
			return;
		}
		cv::Mat3b& innerMat = renderTarget.innerMat();
		cv::Rect innerRect(0, 0, innerMat.cols, innerMat.rows);

		auto tl = renderTarget.project(cv::Point2d(_position.x, _position.y));
		auto br = renderTarget.project(cv::Point2d(_position.x + _position.width, _position.y + _position.height));
		cv::Rect2d matRectDst(tl, br);
		
		bool flipVert = tl.y > br.y;
		bool flipHorz = tl.x > br.x;
		if (flipHorz != _flipHorz || flipVert != _flipVert) {
			_flipHorz = flipHorz;
			_flipVert = flipVert;
			updateFlipped();
		}
		Internal::paint(_flipped, innerMat, matRectDst, _interpolation, _mat);
	}
	void updateAutoPosition() {
		if (_autoPosition) {
			_position = cv::Rect2d(0, 0, _mat.cols, _mat.rows);
		}
	}
};

CVPLOT_DEFINE_FUN
Image::Image(const cv::Mat &mat) {
	setMat(mat);
	impl->_autoPosition = true;
}

CVPLOT_DEFINE_FUN
Image::Image(const cv::Mat &mat, const cv::Rect2d &position) {
	setMat(mat);
	setPosition(position);
}

CVPLOT_DEFINE_FUN
Image::~Image() {
}

CVPLOT_DEFINE_FUN
Image& Image::setMat(const cv::Mat & mat){
	impl->_mat = mat;
    impl->_matBgr = Imp::toBgr(impl->_mat, impl->_nanColor); //ref-copy when bgr, clone otherwise
	impl->updateFlipped();
	impl->updateAutoPosition();
	return *this;
}

CVPLOT_DEFINE_FUN
cv::Mat Image::getMat() const{
	return impl->_mat;
}

CVPLOT_DEFINE_FUN
Image& Image::setPosition(const cv::Rect2d & position){
	impl->_position = position;
	impl->_autoPosition = false;
	return *this;
}

CVPLOT_DEFINE_FUN
cv::Rect2d Image::getPosition(){
	return impl->_position;
}

CVPLOT_DEFINE_FUN
Image & Image::setAutoPosition(bool autoPosition){
	impl->_autoPosition = autoPosition;
	impl->updateAutoPosition();
	return *this;
}

CVPLOT_DEFINE_FUN
bool Image::getAutoPosition()const{
	return impl->_autoPosition;
}

CVPLOT_DEFINE_FUN
Image & Image::setInterpolation(int interpolation){
	impl->_interpolation = interpolation;
	return *this;
}

CVPLOT_DEFINE_FUN
int Image::getInterpolation() const{
	return impl->_interpolation;
}

CVPLOT_DEFINE_FUN
Image& Image::setNanColor(cv::Scalar nanColor){
    if(nanColor==impl->_nanColor){
        return *this;
    }
    impl->_nanColor = nanColor;
    setMat(impl->_mat);
    return *this;
}

CVPLOT_DEFINE_FUN
cv::Scalar Image::getNanColor()const{
    return impl->_nanColor;
}

CVPLOT_DEFINE_FUN
void Image::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool Image::getBoundingRect(cv::Rect2d &rect) {
	if (impl->_mat.empty()) {
		return false;
	}
	rect = impl->_position;
	return true;
}
}
