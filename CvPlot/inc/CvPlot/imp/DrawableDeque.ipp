// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/core/DrawableDeque.h>

namespace CvPlot {

CVPLOT_DEFINE_FUN
DrawableDeque::DrawableDeque() {
}

CVPLOT_DEFINE_FUN
DrawableDeque::DrawableDeque(DrawableDeque && a)
	:_drawables(std::move(a._drawables)) {
}

CVPLOT_DEFINE_FUN
DrawableDeque& DrawableDeque::operator=(DrawableDeque&& a) {
	_drawables = std::move(a._drawables);
	return *this;
}

CVPLOT_DEFINE_FUN
DrawableDeque::Drawables& DrawableDeque::drawables() {
	return _drawables;
}

CVPLOT_DEFINE_FUN
DrawableDeque::Drawables::iterator DrawableDeque::findIterator(Drawable &drawable) {
	return std::find_if(_drawables.begin(), _drawables.end(), [&](const std::unique_ptr<Drawable> &ptr) {
		return ptr.get() == &drawable;
	});
}

CVPLOT_DEFINE_FUN
bool DrawableDeque::getBoundingRect(cv::Rect2d &rect) {
    //combine rects of all items, ignore non-finite values
	bool hasRect = false;
	for (const auto &drawable : _drawables) {
		cv::Rect2d r;
		if (!drawable->getBoundingRect(r)) {
			continue;
		}
        if (!hasRect) {
            rect = r;
            hasRect = true;
            continue;
        }
        if (std::isfinite(r.x) && std::isfinite(r.width)) {
            double x = std::min(rect.x, r.x);
            rect.width = std::max(rect.x + rect.width, r.x + r.width) - x;
            rect.x = x;
        }
        if (std::isfinite(r.y) && std::isfinite(r.height)) {
            double y = std::min(rect.y, r.y);
            rect.height = std::max(rect.y + rect.height, r.y + r.height) - y;
            rect.y = y;
        }
	}
	return hasRect;
}

}
