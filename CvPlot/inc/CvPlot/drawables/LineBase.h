// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/internal/Pimpl.h>
#include <CvPlot/internal/no_warning.h>
#include <CvPlot/core/Drawable.h>
#include <CvPlot/drawables/MarkerType.h>
#include <CvPlot/drawables/LineType.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE LineBase : public Drawable {
public:
	LineBase(const std::string &lineSpec = "-");
	~LineBase();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
	LineBase& setLineSpec(const std::string &lineSpec);
	LineBase& setLineType(LineType lineType);
	LineBase& setColor(cv::Scalar color);
	LineBase& setLineWidth(int lineWidth);
	LineType getLineType();
	cv::Scalar getColor();
	int getLineWidth();
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

template<typename Derived>
class LineBaseSub :public LineBase {
public:
	using LineBase::LineBase;
	Derived& setLineSpec(const std::string &lineSpec) {
		return static_cast<Derived&>(LineBase::setLineSpec(lineSpec));
	}
	Derived& setLineType(LineType lineType) {
		return static_cast<Derived&>(LineBase::setLineType(lineType));
	}
	Derived& setColor(cv::Scalar color) {
		return static_cast<Derived&>(LineBase::setColor(color));
	}
	Derived& setLineWidth(int lineWidth) {
		return static_cast<Derived&>(LineBase::setLineWidth(lineWidth));
	}
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/LineBase.ipp>
#endif