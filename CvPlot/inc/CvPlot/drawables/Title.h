// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/core/Drawable.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Title : public Drawable {
public:
	Title(const std::string &title="");
	~Title();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
	Title& setTitle(const std::string &title);
	std::string getTitle();
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}
#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Title.ipp>
#endif
