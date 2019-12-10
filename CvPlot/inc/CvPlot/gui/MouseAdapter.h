// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/internal/Pimpl.h>
#include <CvPlot/internal/no_warning.h>
#include <CvPlot/core/Axes.h>
#include <CvPlot/gui/MouseEvent.h>
#include <opencv2/highgui.hpp>
#include <functional>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE MouseAdapter {
public:
    MouseAdapter(Axes &axes);
    MouseAdapter(MouseAdapter &&a);
	~MouseAdapter();
	Axes &getAxes();
	bool mouseEvent(const MouseEvent &mouseEvent);
	void setMouseEventHandler(MouseEventHandler mouseEventHandler);
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/MouseAdapter.ipp>
#endif