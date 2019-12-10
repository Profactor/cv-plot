// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/core/Drawable.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Image : public Drawable {
public:
	Image(const cv::Mat &mat = cv::Mat());
	Image(const cv::Mat &mat, const cv::Rect2d &position);
	~Image();
	Image& setMat(const cv::Mat &mat);
	cv::Mat getMat()const;
	Image& setPosition(const cv::Rect2d &position);
	cv::Rect2d getPosition();
	Image &setAutoPosition(bool autoPosition = true);
	bool getAutoPosition()const;
	Image &setInterpolation(int interpolation);
	int getInterpolation()const;
    Image& setNanColor(cv::Scalar nanColor);
    cv::Scalar getNanColor()const;
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Image.ipp>
#endif