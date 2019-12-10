// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/internal/no_warning.h>
#include <CvPlot/core/Drawable.h>
#include <CvPlot/internal/Pimpl.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Border : public Drawable {
public:
	Border();
	~Border();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Border.ipp>
#endif
