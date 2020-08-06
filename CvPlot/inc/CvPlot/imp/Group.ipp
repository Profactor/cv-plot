// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/Group.h>

namespace CvPlot {

CVPLOT_DEFINE_FUN
void Group::render(RenderTarget &renderTarget) {
    for (const auto &drawable : drawables()) {
        drawable->render(renderTarget);
    }
}

CVPLOT_DEFINE_FUN
bool Group::getBoundingRect(cv::Rect2d &rect) {
    return DrawableDeque::getBoundingRect(rect);
}

}
